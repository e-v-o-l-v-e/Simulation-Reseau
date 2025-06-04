#ifndef STP_H
#define STP_H

#include <stdint.h>

#define STATION 1
#define SWITCH  2
#define HUB     0

typedef struct bpdu{
    int id_root;
    int id_envoie;
    int cout;
} bpdu;

typedef struct machine Machine;

//existait deja
typedef struct association {
    int port;
    Machine *cible;
    struct association *suivante;
} association;


//existait deja
struct machine {
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
};

struct etat_port {
  int etat=1;  //par defaut en mode designé // 0 root - 1 désigné - 2 bloqué
  int id_connecte;  // id de la machine qui est connecte en face
};


bpdu creerBPDU(Machine switch);
void receptionBPDU(Machine *sw, bpdu bpdu, uint port_reception, int poids);
int stp(Network *net);

#endif
