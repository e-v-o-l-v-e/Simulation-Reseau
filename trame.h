#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "network.h"

typedef struct trame_ethernet {
    uint8_t preambule[7];
    uint8_t sfd;
    mac dest;
    mac src;  
    uint16_t type; 
    size_t data_length;
    uint8_t *data;
    uint8_t fcs[4];
} trame_ethernet;

trame_ethernet init_trame(mac mac_src, mac mac_dest, uint16_t type, const char* message);
void deinit_trame(trame_ethernet* t);

void afficher_trame(trame_ethernet* t);
void afficher_trame_hexa(trame_ethernet* t);
void type_to_str(char* protocole, uint16_t type);

void envoyer_trame(network* net, mac adr_src, mac adr_dst, const char* message, const char* protocole);