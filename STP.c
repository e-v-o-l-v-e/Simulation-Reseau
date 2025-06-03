#include "STP.h"

bpdu creerBPDU(machine sw) {
    bpdu b;
    b.id_root = sw.id_root;
    b.id_envoie = sw.id;
    b.cout = sw.cout;
    return b;
}

void receptionBPDU(machine *sw, bpdu bpdu, uint port_reception, int poids) {
    int newCost = bpdu.cout + poids;

    if (bpdu.id_root < sw->id_root || (bpdu.id_root == sw->id_root &&
        newCost < sw->cout) || (bpdu.id_root == sw->id_root &&
        newCost == sw->cout && bpdu.id_envoie < sw->port_root)) {

        sw->id_root = bpdu.id_root;
        sw->etat_ports[port_reception].etat = 0;
        sw->cout = newCost;
        sw->port_root = port_reception;
    }
}

int stp(network *net) {
    // init des id root de chaque
    for(size_t i=0; i<net->nbEquipements; i++){
      if (net->equipements[i].type==2){
        net->equipements[i].id_root = net->equipements[i].id;
      }
    }

    // cherche le root (echange de bpdu)
    bool continuer = true;
    while (continuer) {
      //Creation des BPDU
      bpdu* tab_bpdu = malloc(sizeof(bpdu));

      if(tab_bpdu == NULL){
        printf("Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
      }

      size_t nbBPDU = 1;
      for(size_t i = 0; i< net->nbEquipements; i++){
        if (net->equipements[i].type==2){
          bpdu d = creerBPDU(net->equipements[i]);
          tab_bpdu[nbBPDU-1] = d;
          nbBPDU++;

          // realloue la memoire pour le tableau de BPDU
          tab_bpdu = (bpdu*) realloc(tab_bpdu, nbBPDU * sizeof(bpdu));

          if(tab_bpdu == NULL){
            printf("Erreur d'allocation memoire\n");
            exit(EXIT_FAILURE);
          }
        }

        // echange de BPDU
        for(size_t i = 0; i< net->nbEquipements; i++){
          machine envoyeur = net->equipements[i];
          if (envoyeur.type==2){
            // recupere le bpdu du switch
            bpdu d;
            for(size_t j = 0; j<nbBPDU; j++){
              if(tab_bpdu[j].id_envoie == net->equipements[i].id){
                d = tab_bpdu[j];
              }
            }

            sommet* sommet_adj = malloc(net->nbEquipements * sizeof(sommet));

            if(sommet_adj == NULL){
              printf("Erreur d'allocation memoire\n");
              exit(EXIT_FAILURE);
            }

            size_t nb_adj = sommets_adjacents(net->g, i, sommet_adj);
            for(size_t j=0; j< nb_adj; j++){
              machine voisin = net->equipements[sommet_adj[j]];
              if(voisin.type == 2){

                uint port;
                for (size_t k = 0; k<voisin.nbAsso; k++){
                  if (voisin.table[k].adr_mac == envoyeur.adr_mac) {
                    port = voisin.table[k].num_port;
                    break;
                  }
                }

                uint poids = poids_arete(net->g, i, sommet_adj[j]);
                receptionBPDU(&voisin, d, port, poids);
              }
            }

            free(sommet_adj);
          }
        }

      }


      free(tab_bpdu);
      // regarde s'ils ont tous le meme id_root
      bool trouve = false;

      for (size_t i =0; i<net->nbEquipements-1; i++) {
        if (net->equipements[i].type!=2){
          break;
        }
        for(size_t j = i+1; j<net->nbEquipements; j++) {
          if(net->equipements[j].type ==2){
            if (net->equipements[i].id_root != net->equipements[j].id_root) {
              trouve = true;
            }
          }
        }

        // si ils ont tous le meme, on arrete l'echange de bpdu
        if(!trouve){
          continuer = false;
        }
      }

      // changements etat des ports
      for (size_t i=0; i<net->nbEquipements; i++){
        machine sw = net->equipements[i];
        if (sw.type==2){
          for (size_t j=0; j<sw.nb_ports; j++) {
            uint port = j;
            if(sw.port_root == port){
              continue;
            }
            // si c'est pas un port root, on veut changer son etat


            // on recupere l'equipement en face
            int id_equip_connec = sw.etat_ports[port].id_connecte;

            machine switchFace = net->equipements[id_equip_connec];

            if(switchFace.type != 2){
              continue;
            }

            // si c'est un switch on regarde le port qui le relie et on definie
            for(size_t k =0; k<switchFace.nb_ports; k++){
              if(switchFace.etat_ports[k].id_connecte == i){
                if(switchFace.port_root == k){
                  sw.etat_ports[port].etat = 1;   // si c'est un port root on le met en designe
                }
                else{
                  sw.etat_ports[port].etat = 2;   // sinon en bloque
                }
              }
            }

          }
        }
      }

    }

    exit(EXIT_SUCCESS);

}
