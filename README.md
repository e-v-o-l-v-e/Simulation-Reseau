# 🌐 Simulation Réseau (C)
## 📄 Description

**Simulation de réseau** en C en console.
Le programme gère des machines avec adresses MAC, permet l’**envoi de trames** entre ordinateurs et implémente le **protocole STP**.

## 🛠️ Technologies

**Langage :** C

**Concepts :** Ethernet, adresses MAC, protocole STP, communication entre machines

## ⚙️ Installation et lancement

1. Cloner le repository :
```bash
git clone https://github.com/AmbreRoise/Simulation_reseau.git
```
2. Sélectionner le fichier de configuration (ligne 11 du fichier network.c) -> voir plus bas les fichiers possibles:
```c
config = fopen("config1", "r");      //Remplacer config1 par le nom du fichier souhaité
```
3. Modifier le main en conséquence (en fonction du fichier choisi - se fier aux commentaires du main, mettre en commentaire la configuration non choisie)

4. Compiler le programme :
```bash
make
```

5. Lancer le programme
```bash
./main.o
```

## 🚀 Fonctionnalités (Cahier des charges)

📂 **Configurations prédéfinies :**

- config1 : réseau simple
- config2 : réseau plus complexe sans cycle
- config3 : réseau plus complexe avec cycle
- config_cyle.lan : semblable a config3

🖥️ **Structure du réseau :** utilisation d’un graphe et d’un tableau d’équipements (indice = sommet du graphe).

⚡ **Équipements variés :** création de machines de différents types (station, switch, hub), avec des champs spécifiques selon le type.

📡 **Transmission de trames :** création de trames et envoi via fonction récursive parcourant le graphe jusqu’à la destination.

🔄 **Protocole STP :** sélection d’un switch root, gestion de l’état des ports de chaque switch, utilisation de BPDU sans passer par les trames.

📝 **Documentation interne :** code commenté pour expliquer chaque étape du fonctionnement.