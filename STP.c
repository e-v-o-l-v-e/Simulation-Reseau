#include "STP.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bpdu creerBPDU(Machine switch, int port) {
    bpdu b;
    b.id_root = switch.id_root;
    b.id_envoie = switch.id;
    b.cout = switch.cout;
    b.port_envoie = port;
    return b;
}

void receptionBPDU(Machine *sw, bpdu bpdu, int port_reception, int poids) {
    int newCost = bpdu.cout + poids;

    if (bpdu.id_root < sw->id_root || (bpdu.id_root == sw->id_root &&
        newCost < sw->cout) || (bpdu.id_root == sw->id_root &&
        newCost == sw->cout && bpdu.id_envoie < sw->port_root)) {

        sw->id_root = bpdu.id_root;
        sw->cout = newCost;
        sw->port_root = port_reception;
    }
}

void stp(Network *net) {
    // la fonction "finale" (vide pour l'instant)
}
