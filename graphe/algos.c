#include "algos.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

size_t degre(graphe const *g, sommet s)
{
    //Retourne le nombre de sommets adjacents à s
    
    //Vérifie si g contient s
    size_t index_s = index_sommet(g, s);
    if(index_s == UNKNOWN_INDEX){
      return UNKNOWN_INDEX;
    }

    sommet* sommets_adj = malloc((ordre(g)-1) * sizeof(sommet));

    size_t degre = sommets_adjacents(g, s, sommets_adj); 

    free(sommets_adj);
    sommets_adj = NULL;

    return degre;

    /*
    Autre solution : (pas de tableau, pas d'appel à sommets_adjacents)

    size_t degre = 0;
    for(size_t i=0; i<nb_aretes(g); i++){
      arete a = g->aretes[i];  //il faudrait une fonction parce que là on touche à la structure
      if(a.s1 == s || a.s2 == s){
        degre++;
      }
    }
    */
}

bool est_regulier(graphe const *g)
{
    //Retourne si tous les sommets ont le même degré ou non

    size_t nb_sommets = ordre(g);

    size_t degre0 = degre(g, 0);
    for(size_t i=1; i<nb_sommets; i++){
      size_t deg = degre(g, i);
      if(deg != degre0){
        return false;
      }
    }

    return true;;
}

void afficher(graphe const *g)
{
    //Affichage du graphe
    
    printf("# sommets = %zu\n", ordre(g));
    printf("# arêtes = %zu\n", nb_aretes(g));
    
    //Affichage des sommets
    printf("--SOMMETS--\n");
    
    for(size_t i=0; i<ordre(g); i++){
      printf("%zu (degré: %zu) <->", i, degre(g, i));
      sommet* sa = malloc(ordre(g) * sizeof(sommet));
      
      size_t nb_adj = sommets_adjacents(g, i, sa);

      for(size_t j=0; j<nb_adj; j++){
        printf("%zu ", *(sa + j));
      }
      printf("\n");

      free(sa);
      sa = NULL;
    }


    //Affichage des arêtes
    printf("--ARÊTES--\n");

    for(size_t i=0; i<ordre(g); i++){
      sommet* sa = malloc(ordre(g) * sizeof(sommet));
      
      size_t nb_adj = sommets_adjacents(g, i, sa);

      for(size_t j=0; j<nb_adj; j++){
        printf("%zu - %zu\n", i, *(sa + j));
      }

      free(sa);
      sa = NULL;
    }

}

void generer_complet(graphe *g, size_t ordre)
{
    //Insère dans g le nombre de sommets correspondant à ordre
    //Créer les arêtes

    for(size_t i=0; i<ordre; i++){
      ajouter_sommet(g);
    }


    for(size_t i=0; i<ordre; i++){
      for(size_t j=i+1; j<ordre; j++){
        arete a = {i, j};
        ajouter_arete(g, a);
      }
    }

}

void visite_composante_connexe(graphe const *g, sommet s, bool *visite)
{
  //Parcours récursivement les sommets adjacents de s et met à vrai dans visite s'il a été visité
  *(visite + s) = true;
  sommet* sa = malloc(ordre(g) * sizeof(sommet));
  size_t nb_adj = sommets_adjacents(g, s, sa);

  for(size_t i=0; i<nb_adj; i++){
    if(*(visite + *(sa +i)) == false){
      visite_composante_connexe(g, *(sa + i), visite);
    }
  } 
  
  free(sa);
  sa = NULL;
}

uint32_t nb_composantes_connexes(graphe const *g)
{
    //Calcul le nombre de composantes connexes du graphe
    
    bool* visite = malloc(ordre(g) * sizeof(bool));

    for(size_t i=0; i<ordre(g); i++){
      visite[i] = false;
    }

    uint32_t nbCompo = 0;

    for(size_t i=0; i<ordre(g); i++){
      if(!visite[i]){
        nbCompo++;
        visite_composante_connexe(g, i, visite);
      }  
    }

    free(visite);
    visite = NULL;

    return nbCompo;
}

bool sont_connectes(graphe const *g, sommet s1, sommet s2)
{
    //Regarde si deux sommets sont connectés par un chemin

    bool* visite = malloc(ordre(g) * sizeof(bool));

    for(size_t i=0; i<ordre(g); i++){
      visite[i] = false;
    }

    visite_composante_connexe(g, s1, visite);

    if(visite[s2]){
      free(visite);
      visite = NULL;
      return true;
    }

    free(visite);
    visite = NULL;

    return false;
}

void coloriage_glouton(graphe const *g, uint8_t *couleur_sommet)
{
    //Coloriage glouton du graphe

    //Initialise le tableau couleur_sommet (255 à chaque sommet)
    for(size_t i=0; i<ordre(g); i++){
      couleur_sommet[i] = 255;
    }
    
    uint8_t couleur_max = 0;
    couleur_sommet[0] = 0;

    for(size_t i=1; i<ordre(g); i++){
      int* couleurs = malloc(sizeof(int) * (couleur_max+1));

      //init du tableau à 0
      for(uint8_t j=0; j<(couleur_max+1); j++){
        couleurs[j] = 0;
      }

      //Parcours pour le nombre d'occurences de chaque couleur
      sommet* sa = malloc(sizeof(sommet) * ordre(g)-1);
      size_t nb_voisins = sommets_adjacents(g, i, sa);

      for(size_t j=0; j<nb_voisins; j++){
        sommet voisin = sa[j];
        uint8_t couleur_voisin = couleur_sommet[voisin];

        if(couleur_voisin != 255){
          couleurs[couleur_voisin]++;
        }
      }
      
      //trouver la plus petite couleur utilisable
      uint8_t couleur_min = couleur_max +1;
      
      for(uint8_t j=0; j<couleur_min; j++){
        if(couleurs[j] == 0){
          couleur_min = j;
          break;
        }
      }

      if(couleur_min == (couleur_max +1)){
        couleur_max++;
      }

      couleur_sommet[i] = couleur_min;

      free(sa);
      sa = NULL;
      free(couleurs);
      couleurs = NULL;


    }
    
}

void appliquer_permutation(graphe const *src, graphe *dst, size_t const *permutation)
{
}

uint32_t estimation_nb_chromatique(graphe const *g, uint32_t n)
{
    return 0;
}

void generer_aleatoire(graphe *g, size_t ordre, uint32_t k)
{
}

void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet)
{
}

void trier_aretes(arete *aretes_triees, graphe const *g, double const *poids_arete)
{
}

void kruskal(graphe const *g, double const *poids_arete, graphe *acm)
{
}
