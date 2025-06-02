#ifndef STP_H
#define STP_H

#include <stdint.h>

#define STATION 1
#define SWITCH  2
#define HUB     0

typedef struct {
    int id_root;
    int id_envoie;
    int cout;
    int port_envoie;
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

    int id;
    int id_root;
    int cout;
    int port_root;
    int *etat_ports;       // 0 root - 1 désigné - 2 bloqué
};


bpdu creerBPDU(Machine switch, int port);
void receptionBPDU(Machine *sw, bpdu bpdu, int port_reception, int poids);
void stp(Network *net);

#endif
