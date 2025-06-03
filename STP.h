#ifndef STP_H
#define STP_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "graphe.h"

typedef uint8_t mac[6];

typedef struct bpdu{
    int id_root;
    int id_envoie;
    int cout;
} bpdu;

typedef struct association {
    uint num_port;
    mac adr_mac;
} association;

typedef struct etat_port {
  int etat;  //par defaut en mode designé // 0 root - 1 désigné - 2 bloqué
  int id_connecte;  // id de la machine qui est connecte en face
} etat_port;

//existait deja
typedef struct machine {
    unsigned int type;
    char nom[32];
    unsigned char adr_mac[6];
    uint32_t ip;
    int nb_ports;
    unsigned int priorite;
    association *table;
    size_t nbAsso;

    int id;
    int id_root;
    int cout;
    uint port_root;
    etat_port *etat_ports;
} machine;

typedef struct network {
    graphe* g;
    machine* equipements;
    size_t nbEquipements;
}network;

bpdu creerBPDU(machine sw);
void receptionBPDU(machine *sw, bpdu bpdu, uint port_reception, int poids);
int stp(network *net);

#endif
