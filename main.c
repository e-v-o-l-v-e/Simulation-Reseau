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

  printf("==== Affichage des stations ====\n");

  for(size_t i =0; i<reseau->nbEquipements; i++){
    if(reseau->equipements[i].type == 1){
      affiche_infos_station(&reseau->equipements[i]);
      printf("\n");
    }
  }

  printf("\n==== Protocole STP ====\n");
  int retour = stp(reseau);
  if(retour == EXIT_SUCCESS){
    printf("Protocole STP mis en place\n");
  }
  else{
    printf("Protocole STP non réussi\n");
  }

  printf("\n==== Affichage des switchs ====\n");
  for(size_t i =0; i<reseau->nbEquipements; i++){
    if(reseau->equipements[i].type == 2){
      affiche_infos_switch(&reseau->equipements[i]);
      printf("\n");
    }
  }

  printf("\n\n------------------------------\n");
  printf("\n==== Lancement de trames ethernet ====\n");

  //Echange de trame
  /*
  //Config 2
  mac adr_src = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x01};
  mac adr_dst = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x02};
  char* message = "Bonjour";
  char* protocole = "IPv4";
  envoyer_trame(reseau, adr_src, adr_dst, message, protocole);
  printf("---------------\n");
  message = "Coucou";
  envoyer_trame(reseau, adr_dst, adr_src, message, protocole);*/

  
  //Config cycle (config3 et config_cycle)
  mac adr_src = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x01};
  mac adr_dst = {0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x08};
  char* message = "Bonjour";
  char* protocole = "IPv4";
  envoyer_trame(reseau, adr_src, adr_dst, message, protocole);



  
  
  //Affichage des tables de commutation
  printf("\n\n------------------------------\n");
  printf("==== Tables de commutation des switchs ====\n");
  
  for(size_t i =0; i<reseau->nbEquipements; i++){
    if(reseau->equipements[i].type == 2){
      printf("Switch %zu :\n", i);
      affiche_table_commutation(&reseau->equipements[i]);
      printf("\n");
    }
  }


  //Free du reseau
  deinit_reseau(reseau);

  return 0;
}
