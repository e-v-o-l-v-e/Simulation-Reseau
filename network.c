#include "network.h"
#include "graphe.h"
#include "STP.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define LINE_LENGHT 64

network* creation_reseau() {
  FILE *config;
  config = fopen("config_cycle.lan", "r");

  if (config == NULL) {
    perror("le fichier de configuration n'existe pas ou n'est pas lisible.");
    return NULL;
  }

  char line[LINE_LENGHT];

  char *header = fgets(line, LINE_LENGHT, config);
  int nombre_machines, nombre_liens;
  sscanf(header, "%d %d", &nombre_machines, &nombre_liens);

  network *reseau = malloc(sizeof(network));
  reseau->nbEquipements = nombre_machines;
  reseau->equipements = malloc(reseau->nbEquipements * sizeof(machine));
  reseau->g = malloc(sizeof(graphe));
  reseau->g = malloc(sizeof(graphe));
  init_graphe(reseau->g);
  graphe *g = reseau->g;

  for (int i = 0; i < reseau->nbEquipements; i++) {
    ajouter_sommet(g);
    fgets(line, LINE_LENGHT, config);
    int type, nombre_ports, priorite;
    char buffer_mac[32], buffer_ip[32];
    sscanf(line, "%d;", &type);
    reseau->equipements[i].type = type;

    switch (type) {
    case 1:
      sscanf(line, "1;%[^;];%[^;];%d;%d", buffer_mac, buffer_ip, &nombre_ports,
             &priorite);
      string_to_mac(buffer_mac, reseau->equipements[i].adr_mac);
      string_to_ip(buffer_ip, &reseau->equipements[i].adr_ip);
      reseau->equipements[i].id = i;
      break;

    case 2:
      sscanf(line, "2;%[^;];%d;%d", buffer_mac, &nombre_ports, &priorite);
      string_to_mac(buffer_mac, reseau->equipements[i].adr_mac);
      reseau->equipements[i].nb_ports = nombre_ports;
      reseau->equipements[i].priorite = priorite;
      reseau->equipements[i].id = i;
      reseau->equipements[i].table = malloc(sizeof(association) * (reseau->nbEquipements-1));


      //on alloue la memoire pour les etats_ports
      reseau->equipements[i].etat_ports = malloc(nombre_ports * sizeof(etat_port));

      //on les met tous en etat désigné
      for(size_t j=0; j<nombre_ports; j++){
        reseau->equipements[i].etat_ports[j] = (etat_port) {1, -1};
      }

      //init leur id du root
      reseau->equipements[i].stp_root |= ((uint64_t) reseau->equipements[i].priorite) << 48;

      for (int j = 0; j < 6; j++) {
        reseau->equipements[i].stp_root |= ((uint64_t)reseau->equipements[i].adr_mac[j]) << (40 - 8 * j);
      }

      break;

    case 0:
      sscanf(line, "0;%d", &nombre_ports);
      reseau->equipements[i].nb_ports = nombre_ports;
      reseau->equipements[i].id = i;
      break;
    }
  };

  for (int i = 0; i < nombre_liens; i++) {
    fgets(line, LINE_LENGHT, config);
    unsigned int s1, s2, poid;
    sscanf(line, "%d;%d;%d", &s1, &s2, &poid);
    arete a = {s1, s2, poid};
    ajouter_arete(g, a);

    //ajout de l'id dans etat_port (s1)
    machine equip = reseau->equipements[s1];
    for(size_t j=0; j<equip.nb_ports; j++){
      if(equip.type == 2 && equip.etat_ports[j].id_connecte == -1){
        reseau->equipements[s1].etat_ports[j].id_connecte = s2;
        break;  // on a trouve le premier etat port pas init
      }
    }

    //ajout de l'id dans etat_port (s2)
    machine equip2 = reseau->equipements[s2];
    for(size_t j=0; j<equip2.nb_ports; j++){
      if(equip2.type == 2 && equip2.etat_ports[j].id_connecte == -1){
        reseau->equipements[s2].etat_ports[j].id_connecte = s1;
        break;  // on a trouve le premier etat port pas init
      }
    }

  }

  return reseau;

}

void deinit_reseau(network* reseau){
  //Free de la table d'asso et des etats ports des switchs
  for(size_t i=0; i<reseau->nbEquipements; i++){
    if(reseau->equipements[i].type == 2){
      free(reseau->equipements[i].table);
      reseau->equipements[i].table = NULL;
      free(reseau->equipements[i].etat_ports);
      reseau->equipements[i].etat_ports = NULL;
    }
  }

  free(reseau->equipements);
  reseau->equipements = NULL;

  deinit_graphe(reseau->g);
  reseau->g = NULL;

  free(reseau);
  reseau = NULL;
}

void string_to_mac(const char *adr, uint8_t mac[6]) {
  sscanf(adr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mac[0], &mac[1], &mac[2],
         &mac[3], &mac[4], &mac[5]);
}

void string_to_ip(const char *adr, uint32_t *ip) {
  unsigned int a, b, c, d;
  sscanf(adr, "%u.%u.%u.%u", &a, &b, &c, &d);
  *ip = (a << 24) | (b << 16) | (c << 8) | d;
}

char *ip_to_string(const uint32_t ip){
  char *str = malloc(16);
  sprintf(str, "%u.%u.%u.%u", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
  return str;
}

char *mac_to_string(const mac m) {
  char *str = malloc(18);
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", m[0], m[1], m[2], m[3], m[4], m[5]);
  return str;
}

char *mac_to_string_hexa(const mac m) {
  char *str = malloc(18);
  sprintf(str, "%02x %02x %02x %02x %02x %02x", m[0], m[1], m[2], m[3], m[4], m[5]);
  return str;
}

void afficher(network* reseau) {
  if(reseau == NULL){
    printf("nullll\n");
  }

  printf("# machines = %zu\n", ordre(reseau->g));
  printf("# liens = %zu\n", nb_aretes(reseau->g));

  // Affichage des sommets
  printf("--EQUIPEMENTS--\n");

  for (size_t i = 0; i < ordre(reseau->g); i++) {
    switch (reseau->equipements[i].type) {
    case 1:
      printf("%zu : station, adresse mac : %s (connecté avec : ", i,
              mac_to_string(reseau->equipements[i].adr_mac));
      break;

    case 2:
      printf("%zu : switch, adresse mac : %s (connecté avec : ", i,
              mac_to_string(reseau->equipements[i].adr_mac));
      break;

    case 0:
      printf("%zu : hub, connecté à :", i);
      break;
    }
    sommet *sa = malloc(ordre(reseau->g) * sizeof(sommet));

    size_t nb_adj = sommets_adjacents(reseau->g, i, sa);

    for (size_t j = 0; j < nb_adj; j++) {
      printf("%zu ", *(sa + j));
    }
    printf(")\n");

    free(sa);
    sa = NULL;
  }

  // Affichage des arêtes
  printf("--LIENS--\n");

  for (size_t i = 0; i < ordre(reseau->g); i++) {
    sommet *sa = malloc(ordre(reseau->g) * sizeof(sommet));

    size_t nb_adj = sommets_adjacents(reseau->g, i, sa);

    for (size_t j = 0; j < nb_adj; j++) {
      printf("%zu - %zu\n", i, *(sa + j));
    }

    free(sa);
    sa = NULL;
  }
  
  printf("\n");
}

size_t degre(graphe const *g, sommet s) {
  // Retourne le nombre de sommets adjacents à s
  size_t index_s = index_sommet(g, s);
  if (index_s == UNKNOWN_INDEX) {
    return UNKNOWN_INDEX;
  }

  sommet *sommets_adj = malloc((ordre(g) - 1) * sizeof(sommet));

  size_t degre = sommets_adjacents(g, s, sommets_adj);

  free(sommets_adj);
  sommets_adj = NULL;

  return degre;
}

bool existe_machine(network* net, const mac adr){
  //Verifie si un equipement du reseau possede cette adresse mac (s'il existe)

  for(size_t i =0; i<net->nbEquipements; i++){
    machine equip = net->equipements[i];
    if (memcmp(equip.adr_mac, adr, 6) == 0) {
        return true;
    }
  }

  return false;
}

int existe_asso(machine* sw, mac adr_mac) {
  for (int i = 0; i < sw->nb_ports; i++) { //pour chaque case de notre table
      if (memcmp(sw->table[i].adr_mac, adr_mac, 6) == 0) { //si on a la meme adresse mac
          return (int)sw->table[i].num_port; //on retroune le port associe a cette adr
      }
  }
  return -1; // Pas trouve
}


void ajout_asso(machine* sw, mac adr_mac, uint port) {
  sw->table[sw->nbAsso].num_port = port;
  memcpy(sw->table[sw->nbAsso].adr_mac, adr_mac, 6); //copie 6 octets
  sw->nbAsso++;
}

void affiche_table_commutation(machine* sw){
  printf("   Table de commutation :\n");
  if(sw->nbAsso >0){
    for(size_t i=0; i<sw->nbAsso; i++){
      printf("     Port %d : %s\n", sw->table[i].num_port, mac_to_string(sw->table[i].adr_mac));
    }
  }
  else{
    printf("      ...\n");
  }
}

void affiche_port_switch(machine* sw){
  if(sw->id == sw->id_root){
    printf("   Switch racine : Oui\n");
  }
  else{
    printf("   Switch racine : Non\n");
  }

  printf("   Coût jusqu'à la racine : %d\n", sw->cout);

  printf("   Etats des ports (%d) :\n", sw->nb_ports);
  for(size_t i=0; i<sw->nb_ports; i++){
    if(sw->etat_ports[i].id_connecte != -1){
      char* etat = "";
      if(sw->etat_ports[i].etat == 0){
        etat = "Racine";
      }
      else if(sw->etat_ports[i].etat == 1){
        etat = "Désigné";
      }
      else{
        etat = "Bloqué";
      }
      printf("      Port %zu relié à l'équipement %zu : %s\n", i, sw->etat_ports[i].id_connecte, etat);
    }
  }
  printf("\n");
}

void affiche_infos_station(machine* station){
  printf("Station %zu :\n", station->id);
  printf("   Adresse IP : %s\n", ip_to_string(station->adr_ip));
  printf("   Adresse MAC : %s\n", mac_to_string_hexa(station->adr_mac));
}

void affiche_infos_switch(machine* sw){
  printf("Switch %zu :\n", sw->id);
  printf("   Adresse IP : %s\n", ip_to_string(sw->adr_ip));
  printf("   Adresse MAC : %s\n", mac_to_string_hexa(sw->adr_mac));
  printf("\n");

  affiche_port_switch(sw);
  affiche_table_commutation(sw);
}