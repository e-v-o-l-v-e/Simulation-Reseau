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
    sommet id_root;
    uint64_t stp_root;
    sommet id_envoie;
    int cout;
} bpdu;

int stp(network *net);
