#include <stdio.h>
#include "trame.h"

int main(){
    mac mac_src  = {0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB};
    mac mac_dest = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

    trame_ethernet t = init_trame(mac_src, mac_dest, 0x0800, "Bonjour");

    afficher_trame(&t);
    afficher_trame_hexa(&t);

    deinit_trame(&t);
    return 0;
}