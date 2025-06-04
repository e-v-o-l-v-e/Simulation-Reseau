#include "trame.h"
#include <stdlib.h>

uint16_t str_to_type(const char* protocole);
bool parcours_switch_recursif(network* net, machine* equip, trame_ethernet* t);    //Retourne true si la trame est arrivée, false sinon

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

void afficher_mac_user(uint8_t mac[6]){
   printf("%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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
   switch (protocole)
   {
      case "IPv4":
         return 0x0800;
      
      case "ARP":
         return 0x0806;
      
      case "IPv6":
         return 0x86DD;

      default:
         return 0;
   }
}

bool parcours_switch_recursif(network* net, machine* equip, trame_ethernet* t){

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
      if(equip.adr_mac == adr_src){                         //Si c'est la station source
         src = i;
         break;
      }
   }


   //Cherche le switch qui est connecté à la station source
   machine* sw == NULL;
   for(size_t i =0; i<net->nbEquipements; i++){
      machine equip = net->equipements[i];
      if(equip.type == 2){                         //Si c'est un switch
         arete a = (arete) {src, i};
         if(existe_arete(net->g, a)){
            sw = &net->equipements[i];
         }
      }
   }

   if(sw == NULL){
      printf("La station source n'est pas relié à un switch\n");
      return;
   }


   //Appel à la fonction d'envoi
   printf("Envoie de la trame...\n");
   bool reussite = parcours_switch_recursif(net, sw, &t);

   //Affiche le retour
   if(reussite){
      printf("Trame envoyée et reçue avec succès !\n");
   }
   else{
      printf("Quelque chose s'est mal passé... La trame n'a pas été reçue\n");
   }

   //Fin de l'envoi de la trame, on la deinit
   deinit_trame(&t);
}
