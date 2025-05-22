# SAE_S21

struct network {

    - graphe* g

    - machine* equipements

    - size_t nbEquipements

}

struct arete {

    - sommet s1

    - sommet s2

    - uint poids

}

struct machine {

    - uint type (1 : station, 2 : switch, 0 : hub)

    - char[32] nom

    - mac adr_mac         //1, 2

    - uint32_t ip         //1

    - int nb_ports        //0, 2

    - uint priorite       //2

    - association* table  //2

}

struct association {

    - uint num_port

    - mac adr_mac

}

define mac unsigned char
