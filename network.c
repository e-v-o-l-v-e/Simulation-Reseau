#include "network.h"
#include "graphe.h"
#include "STP.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define LINE_LENGHT 64

void creation_reseau() {
  FILE *config;
  config = fopen("config2", "r");

  if (config == NULL) {
    perror("le fichier de configuration n'existe pas ou n'est pas lisible.");
    return;
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
      break;

    case 2:
      sscanf(line, "2;%[^;];%d;%d", buffer_mac, &nombre_ports, &priorite);
      string_to_mac(buffer_mac, reseau->equipements[i].adr_mac);
      reseau->equipements[i].nb_ports = nombre_ports;
      reseau->equipements[i].priorite = priorite;
      reseau->equipements[i].id = i;

      //on alloue la memoire pour les etats_ports
      reseau->equipements[i].etat_ports = malloc(nombre_ports * sizeof(etat_port));

      //on les met tous en etat inconnu
      for(size_t j=0; j<nombre_ports; j++){
        reseau->equipements[i].etat_ports[j] = (etat_port) {-1, -1};
      }
      break;

    case 0:
      sscanf(line, "0;%d", &nombre_ports);
      reseau->equipements[i].nb_ports = nombre_ports;
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
      if(equip.etat_ports[j].id_connecte == -1){
        reseau->equipements[s1].etat_ports[j].id_connecte = s2;
        break;  // on a trouve le premier etat port pas init
      }
    }

    //ajout de l'id dans etat_port (s2)
    machine equip2 = reseau->equipements[s2];
    for(size_t j=0; j<equip2.nb_ports; j++){
      if(equip2.etat_ports[j].id_connecte == -1){
        reseau->equipements[s2].etat_ports[j].id_connecte = s1;
        break;  // on a trouve le premier etat port pas init
      }
    }


  }

  afficher(*reseau);

  int retour = stp(reseau);
  if(retour == EXIT_SUCCESS){
    printf("Protocole STP mis en place\n");
  }
  else{
    printf("Protocole STP non reussi\n");
  }
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
char *mac_to_string(const mac m) {
  char *str = malloc(18);
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", m[0], m[1], m[2], m[3], m[4], m[5]);
  return str;
}

  void afficher(network reseau) {

    graphe *g = reseau.g;

    printf("# machines = %zu\n", ordre(g));
    printf("# liens = %zu\n", nb_aretes(g));

    // Affichage des sommets
    printf("--EQUIPEMENTS--\n");

    for (size_t i = 0; i < ordre(g); i++) {
      switch (reseau.equipements[i].type) {
      case 1:
        printf("%zu : station, adresse mac : %s (connecté avec : ", i,
               mac_to_string(reseau.equipements[i].adr_mac));
        break;

      case 2:
        printf("%zu : switch, adresse mac : %s (connecté avec : ", i,
               mac_to_string(reseau.equipements[i].adr_mac));
        break;

      case 0:
        printf("%zu : hub, connecté à :", i);
        break;
      }
      sommet *sa = malloc(ordre(g) * sizeof(sommet));

      size_t nb_adj = sommets_adjacents(g, i, sa);

      for (size_t j = 0; j < nb_adj; j++) {
        printf("%zu ", *(sa + j));
      }
      printf(")\n");

      free(sa);
      sa = NULL;
    }

    // Affichage des arêtes
    printf("--LIENS--\n");

    for (size_t i = 0; i < ordre(g); i++) {
      sommet *sa = malloc(ordre(g) * sizeof(sommet));

      size_t nb_adj = sommets_adjacents(g, i, sa);

      for (size_t j = 0; j < nb_adj; j++) {
        printf("%zu - %zu\n", i, *(sa + j));
      }

      free(sa);
      sa = NULL;
    }
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
