#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t mac[6];

typedef struct trame_ethernet {
    uint8_t preambule[7];
    uint8_t sfd;
    mac dest;
    mac src;  
    uint16_t type; 
    uint8_t *data;
    size_t data_length;
    uint8_t fcs[4];
} trame_ethernet;

trame_ethernet init_trame(mac mac_src, mac mac_dest, uint16_t type, const char* message);
void deinit_trame(trame_ethernet* t);

void afficher_trame(trame_ethernet* t);
void afficher_trame_hexa(trame_ethernet* t);
void afficher_mac_user(uint8_t mac[6]);
void donne_type(char* protocole, uint16_t type);
