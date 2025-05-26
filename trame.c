#include "trame.h"

trame_ethernet init_trame(mac mac_src, mac mac_dest, uint16_t type, const char* message){
   trame_ethernet t = {
      .preambule = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
      .sfd = 0xAB,
      .type = type,
      .data_length = strlen(message) + 1,
      .data = malloc(t.data_length),
      .fcs = {0, 0, 0, 0}
   };

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
   donne_type(protocole, t->type);
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

void donne_type(char* protocole, uint16_t type){
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

