#pragma once

#include "graphe.h"

typedef uint8_t mac[6];

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
    mac adr_mac;
    uint32_t adr_ip;
    int nb_ports;
    unsigned int priorite;
    uint64_t stp_root;
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
} network;


void creation_reseau();
void afficher(network reseau);
char *mac_to_string(const mac m);
void string_to_mac(const char *adr, uint8_t mac[6]);
void string_to_ip(const char *adr, uint32_t *ip);
size_t degre(graphe const *g, sommet s);
