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

    - uint type (0 : station, 1 : switch, 2 : hub)

    - char[32] nom

    - mac adr_mac         //0, 1

    - uint32_t ip         //0

    - int nb_ports        //1, 2

    - uint priorite       //1

    - association* table  //1

}

struct association {

    - uint num_port

    - mac adr_mac

}

define mac unsigned char
