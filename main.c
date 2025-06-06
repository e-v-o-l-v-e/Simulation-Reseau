#include "network.h"
#include "STP.h"
#include "trame.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  network* reseau = creation_reseau();

  if(reseau == NULL){
    exit(EXIT_FAILURE);
  }

  afficher(reseau);

  int retour = stp(reseau);
  if(retour == EXIT_SUCCESS){
    printf("Protocole STP mis en place\n");
  }
  else{
    printf("Protocole STP non reussi\n");
  }

  //Echange de trame

  //Config 2
  mac adr_src = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x01};
  mac adr_dst = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x02};
  char* message = "Bonjour";
  char* protocole = "IPv4";
  envoyer_trame(reseau, adr_src, adr_dst, message, protocole);
  message = "Coucou";
  envoyer_trame(reseau, adr_dst, adr_src, message, protocole);

  printf("\n============================\n");

  /*Config cycle
  mac adr_src = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x01};
  mac adr_dst = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x08};
  char* message = "Bonjour";
  char* protocole = "IPv4";
  envoyer_trame(reseau, adr_src, adr_dst, message, protocole);*/
  
  //Affichage des tables de commutation
  for(size_t i =0; i<reseau->nbEquipements; i++){
    if(reseau->equipements[i].type == 2){
      affiche_table_commutation(&reseau->equipements[i]);
      printf("\n");
    }
  }

  //Free du reseau
  deinit_reseau(reseau);

  return 0;
}
