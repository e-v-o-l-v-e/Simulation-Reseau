#include "graphe.h"

typedef uint8_t mac[6];

typedef struct etat_port {
  int etat;  //par defaut en mode designé // 0 root - 1 désigné - 2 bloqué
  int id_connecte;  // id de la machine qui est connecte en face
} etat_port;

typedef struct association {
    uint num_port;
    mac adr_mac;
} association;

typedef struct machine {
    uint type;            // (1 : station, 2 : switch, 0 : hub)
    char nom[32];
    mac adr_mac;         //1, 2
    uint32_t adr_ip;         //1
    int nb_ports;        //0, 2
    uint priorite;       //2
    association* table;  //2
    size_t nbAsso;
    int id;
    int id_root;
    uint64_t stp_root;
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
