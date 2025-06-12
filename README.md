# SAE 2.3 Réseau

./config1 : configuration très simple

./config2 : configuration plus complexe sans cycle

./config3 : configuration plus complexe avec cycle

## Explication 

- Utilisation d'un réseau contenant un graphe, un tableau d'équipements (l'indice dans le tableau représente le sommet dans le graphe) et un nombre d'équipements

- Réutilisation du fichier graphe utilisé en M23 avec rajout du poids sur les arêtes

- Création d'équipements déterminés par leur type (station, switch, hub) --> certains champs ne sont accessibles qu'à certains type

- Création de trames avec fonction d'envoi qui appelle elle-même une fonction récursive qui transporte la trame dans le graphe jusqu'à destination

- Création du STP qui choisit un switch root et s'occupe de l'état des ports de chaque switch en conséquence. Utilisation de BPDU sans passer par les trames