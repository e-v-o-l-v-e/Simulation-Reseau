struct machine {
    uint type (1 : station, 2 : switch, 0 : hub)
    char[32] nom
    mac adr_mac         //1, 2
    uint32_t ip         //1
    int nb_ports        //0, 2
    uint priorite       //2
    association* table  //2

    int id              //2
    int id_root         //2
    int cout            //2
    int port_root       //2
    int *etat_ports     //2
    //0 root - 1 designé - 2 bloqué
};

struct bpdu {
    int id_root
    int id_envoie
    int cout
    int port_envoie
};

//fonction creation de bpdu
bpdu creerBPDU (Machine switch, int port) { //on donne un switch qui envoit le bpdu ainsi que le port par lequel il envoit
    bpdu b;
    b.id_root=switch.id_root;
    b.id_envoie=switch.id;
    b.cout=switch.cout;
    b.port_envoie=port;
};

//fonction de reception BPDU
//choix du root
void receptionBPDU (Machine sw, bpdu bpdu, int port_reception, int poids) {
    int newCost=bpdu.cout +poids; //on recup le cout de base et on y ajoute le poid de l'arete

    if (bpdu.id_root<sw.id_root || (bpdu.id_root==sw.id_root &&
                                    newCost < sw.cout ) || (bpdu.id_root == sw.id_root &&
                                                            newCost==sw.cout && bpdu.id_envoie< sw.port_root)) { //toutes situation possibles

      sw.id_root = bpdu.id_root;
      sw.cout=newCost;
      sw.port_root= port_reception;

    }

};

void stp (Network *net) {
    //la fonction "finale"
};
