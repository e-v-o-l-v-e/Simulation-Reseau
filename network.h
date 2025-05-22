#include "graphe.h"
#include <stdint.h>

typedef uint8_t mac[6];

typedef struct association {
    uint num_port;
    mac adr_mac;
} association;

typedef struct machine {
    uint type;            // (1 : station, 2 : switch, 0 : hub)
    char nom[32];
    mac adr_mac;         //1, 2
    uint32_t ip;         //1
    int nb_ports;        //0, 2
    uint priorite;       //2
    association* table;  //2
} machine;

typedef struct network {
    graphe* g;
    machine* equipements;
    size_t nbEquipements;
} network;

typedef struct trame_ethernet {
    uint8_t preambule[7];
    uint8_t sfd;
    mac dest;
    mac src;  
    uint8_t type[2];
    uint8_t *data;
    uint8_t fcs[4];
} trame_ethernet;
