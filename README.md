# SAE 2.3 Réseau

config1 : configuration très simple

config2 : configuration plus complexe sans cycle

config3 : configuration plus complexe avec cycle

```c
typedef uint8_t mac[6];

struct network {
    graphe* g
    machine* equipements
    size_t nbEquipements
};

struct machine {
    uint type (1 : station, 2 : switch, 0 : hub)
    char[32] nom
    mac adr_mac         //1, 2
    uint32_t ip         //1
    int nb_ports        //0, 2
    uint priorite       //2
    association* table  //2
    size_t nbAsso;      //2

    int id;             //2
    int id_root;        //2
    int cout;           //2
    uint port_root;     //2
    etat_port *etat_ports; //2
};

struct etat_port {
  int etat=1;  //par defaut en mode designé // 0 root - 1 désigné - 2 bloqué
  int id_connecte;  // id de la machine qui est connecte en face
};



struct association {
    uint num_port
    mac adr_mac
};

struct trame_ethernet {
    uint8_t preambule[7];
    uint8_t sfd;
    mac dest;
    mac src;  
    uint8_t type[2];
    uint8_t *data;
    uint8_t fcs[4];
};

struct graphe {
  size_t ordre;
  arete *aretes;
  size_t aretes_capacite;
  size_t nb_aretes;
} graphe;

struct arete {
    sommet s1
    sommet s2
    uint poids
};
```
