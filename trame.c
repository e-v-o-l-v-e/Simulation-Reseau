#include "trame.h"
#include <stdlib.h>

uint16_t str_to_type(const char* protocole);
bool parcours_switch_recursif(network* net, machine* equip, sommet id_equip, trame_ethernet* t, uint port);    //Retourne true si la trame est arrivée, false sinon

trame_ethernet init_trame(mac mac_src, mac mac_dest, uint16_t type, const char* message){
   trame_ethernet t;
   memcpy(t.preambule, (uint8_t[]){0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA}, 7);
   t.sfd = 0xAB;
   t.type = type;
   t.data_length = strlen(message) + 1;
   t.data = malloc(t.data_length);
   memset(t.fcs, 0, 4);

   memcpy(t.dest, mac_dest, 6);
   memcpy(t.src, mac_src, 6);
   memcpy(t.data, message, t.data_length);

   return t;
}

void deinit_trame(trame_ethernet* t){
   t->data_length = 0;
   free(t->data);
   t->data = NULL;
}

void afficher_trame(trame_ethernet* t){
   printf("Affichage de la trame...\n");

   afficher_mac_user(t->src);
   printf(" -> ");
   afficher_mac_user(t->dest);
   printf("\n");

   char protocole[32];
   type_to_str(protocole, t->type);
   printf("Type : 0x%04X (%s)\n", t->type, protocole);
   printf("Data : %s\n", (char*) t->data);
   
}

void afficher_trame_hexa(trame_ethernet* t){
   printf("Affichage de la trame en héxadécimal...\n");

   // Préambule
    for (int i = 0; i < 7; i++)
        printf("%02X ", t->preambule[i]);

    // SFD
    printf("%02X ", t->sfd);

    // Adresse MAC destination 
    for (int i = 0; i < 6; i++)
        printf("%02X ", t->dest[i]);

    // Adresse MAC source
    for (int i = 0; i < 6; i++)
        printf("%02X ", t->src[i]);

    // Type
    printf("%02X %02X ", (t->type >> 8) & 0xFF, t->type & 0xFF);

    // Data 
    for (size_t i = 0; i < t->data_length; i++)
        printf("%02X ", t->data[i]);

    // FCS 
    for (int i = 0; i < 4; i++)
        printf("%02X ", t->fcs[i]);

    printf("\n");
}

void afficher_mac_user(mac adr_mac){
   printf("%02X:%02X:%02X:%02X:%02X:%02X", adr_mac[0], adr_mac[1], adr_mac[2], adr_mac[3], adr_mac[4], adr_mac[5]);
}

void type_to_str(char* protocole, uint16_t type){
   switch (type)
   {
      case 0x0800:
         strcpy(protocole, "IPv4");
         break;
      
      case 0x0806:
         strcpy(protocole, "ARP");
         break;
      
      case 0x86DD:
         strcpy(protocole, "IPv6");
         break;

      default:
         strcpy(protocole, "Unknown");
         break;
   }
}

uint16_t str_to_type(const char* protocole){

   if(strcmp(protocole, "IPv4") == 0){
      return 0x0800;
   }
   
   if(strcmp(protocole, "ARP") == 0){
      return 0x0806;
   }

   if(strcmp(protocole, "IPv6") == 0){
      return 0x86DD;
   }

   return 0;
}

uint recup_port(network* net, sommet id_equip_face, sommet id_equip){
   uint port_face = 0;
   
   for(uint i=0; i<net->equipements[id_equip].nb_ports; i++){
      if(net->equipements[id_equip].type == 2 && net->equipements[id_equip].etat_ports[i].id_connecte == id_equip_face){
         port_face = i;
      }
   }

   return port_face;
}

bool parcours_switch_recursif(network* net, machine* equip, sommet id_equip, trame_ethernet* t, uint port){
   printf("\n");
   //Verifie si l'équipement est une station
   if(equip->type == 1){
      if(memcmp(equip->adr_mac, t->dest, 6) == 0){            //S'il s'agit de la sation destinataire
         printf("[Station %zu] Trame reçue ! Destination atteinte.\n", id_equip);
         return true;
      }
      printf("[Station %zu] La trame n'est pas pour cette station.\n", id_equip);
      return false;
   }

   if(equip->type == 0){            //S'il s'agit d'un hub, broadcast
      printf("[Hub %zu] Trame reçue via le port %d : broadcast.\n", id_equip, port);

      for(size_t i =0; i<equip->nb_ports; i++){
         if(i == port){             //Si c'est le port de réception, on envoie pas
            continue;
         }

         sommet id_en_face = equip->etat_ports[i].id_connecte;
         uint port_face = recup_port(net, id_en_face, id_equip);

         if(net->equipements[id_en_face].type == 2 && net->equipements[id_en_face].etat_ports[port_face].etat == 2){       //Si le port est bloqué, on envoie pas
            continue;
         }

         //Envoie de la trame au suivant
         bool essai = parcours_switch_recursif(net, &net->equipements[id_en_face], id_en_face, t, port_face);
         
         if(essai){
            return true;     //Si on a reussi, on met à true
         }

      }

      return false;
   }

   //Côté switch
   printf("[Switch %zu] Trame reçue via le port %d.\n", id_equip, port); 

   //On verifie si l'association port/adr_source existe et on l'ajoure si non
   int port_asso = existe_asso(equip, t->src);

   if(port_asso == -1){
      ajout_asso(equip, t->src, port);
      printf("\tApprentissage : MAC ");
      afficher_mac_user(t->src);
      printf(" -> port %d\n", port);
   }

   //On verifie si une association avec adr_dest existe
   int port_envoie = existe_asso(equip, t->dest);

   if(port_envoie != -1){     //L'association existe
      sommet id_face = equip->etat_ports[port_envoie].id_connecte;
      machine* equip_face = &net->equipements[id_face];

      //Cherche le port en face
      uint port_recep = recup_port(net, id_face, id_equip);
      printf("[Switch %zu] Destination connue : ", id_equip);
      afficher_mac_user(t->src);
      printf(" -> port %d\n", port_recep);

      printf("\t   Port %d -> équipement %zu\n", port_envoie, id_face);

      bool essai = parcours_switch_recursif(net, equip_face, id_face, t, port_recep);

      if(essai){
         return true;
      }
   }
   else{
      for(uint i=0; i<equip->nb_ports; i++){
         if(i != port){                         //On ne renvoie pas sur le port de réception
            sommet id_face = equip->etat_ports[i].id_connecte;
            if(id_face >= net->nbEquipements){        //Si le port n'est connecté à rien
               continue;
            }
            machine* equip_face = &net->equipements[id_face];

            printf("[Switch %zu] Destination inconnu : ", id_equip);
            afficher_mac_user(t->src);
            printf(" -> diffusion\n");
            
            //Cherche le port en face
            uint port_recep = recup_port(net, id_equip, id_face);

            if(net->equipements[id_face].type == 2 && net->equipements[id_face].etat_ports[port_recep].etat == 2){       //Si le port est bloqué, on envoie pas
               continue;
            }

            printf("\t   Port %d -> équipement %zu\n", i, id_face);

            bool essai = parcours_switch_recursif(net, equip_face, id_face, t, port_recep);

            if(essai){
               return true;
            }
            else{
               printf("Retour au switch %zu.\n", id_equip);
            }
         }
      }
   }

   return false;

}

void envoyer_trame(network* net, mac adr_src, mac adr_dst, const char* message, const char* protocole){
   //verifie si les deux stations sont dans le reseau
   if(!existe_machine(net, adr_src)){
      printf("La station de départ n'existe pas dans le réseau\n");
      return;
   }

   if(!existe_machine(net, adr_dst)){
      printf("La station d'arrivée n'existe pas dans le réseau\n");
      return;
   }

   //Création de la trame
   uint16_t type = str_to_type(protocole);
   trame_ethernet t = init_trame(adr_src, adr_dst, type, message);

   //Cherche le numero de sommet de la station source
   sommet src = -1;
   for(size_t i =0; i<net->nbEquipements; i++){
      machine equip = net->equipements[i];
      if(memcmp(equip.adr_mac, adr_src, 6) == 0){                         //Si c'est la station source
         src = i;
         break;
      }
   }

   //Cherche le switch qui est connecté à la station source
   machine* sw = NULL;
   sommet swit_ch = -1;
   for(size_t i =0; i<net->nbEquipements; i++){
      machine equip = net->equipements[i];
      if(equip.type == 2){                         //Si c'est un switch
         arete a = (arete) {src, i, 0};
         if(existe_arete(net->g, a)){
            sw = &net->equipements[i];
            swit_ch = i;
            break;
         }
      }
   }

   if(sw == NULL){
      printf("La station source n'est pas relié à un switch\n");
      return;
   }

   //Cherche le port de réception
   uint port = 0;
   for(uint i =0; i<sw->nb_ports; i++){
      if(sw->etat_ports[i].id_connecte == src){            //Si l'equip enface
         port = i;
      }
   }


   //Appel à la fonction d'envoi
   printf("\n=============================\n");
   printf("Envoie de la trame...\n");
   printf("----------------\n");
   bool reussite = parcours_switch_recursif(net, sw, swit_ch, &t, port);

   //Affiche le retour
   if(!reussite){
      printf("Quelque chose s'est mal passé... La trame n'a pas été reçue\n");
   }

   //Fin de l'envoi de la trame, on la deinit
   deinit_trame(&t);
}
