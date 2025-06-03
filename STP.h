#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "graphe.h"
#include "network.h"

typedef uint8_t mac[6];

typedef struct bpdu{
    int id_root;
    int id_envoie;
    int cout;
} bpdu;

bpdu creerBPDU(machine sw);
void receptionBPDU(machine *sw, bpdu bpdu, uint port_reception, int poids);
int stp(network *net);
