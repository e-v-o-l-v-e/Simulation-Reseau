#include "graphe.h"

#define INITIAL_CAPACITY 8

void normaliser_arete(arete *arete);
void swap_sommets(arete *a);

void init_graphe(graphe *g)
{
    // initialise les champs internes du graphe g
    // - allocation d'un tableau d'arêtes de capacité initiale 8
    // - le graphe ne contient initialement ni sommet ni arête

    g->ordre = 0;
    g->nb_aretes = 0;
    g->aretes_capacite = INITIAL_CAPACITY;
    g->aretes = malloc(g->aretes_capacite * sizeof(arete));

    if(g->aretes == NULL){
      fprintf(stderr, "Erreur d'allocation mémoire\n"); //Sortie erreur
      exit(EXIT_FAILURE);
    }
}

void deinit_graphe(graphe *g)
{
    // libère la mémoire qui avait été allouée dans la fonction init_graphe
    // réinitialise les champs internes du graphe g
    g->ordre = 0;
    g->nb_aretes = 0;
    g->aretes_capacite = 0;
    free(g->aretes);
    g->aretes = NULL;
}

size_t ordre(graphe const *g)
{
    return g->ordre;
}

size_t nb_aretes(graphe const *g)
{
    return g->nb_aretes;
}

void ajouter_sommet(graphe *g)
{
  g->ordre += 1;
}

size_t index_sommet(graphe const *g, sommet s)
{
    // retourne l'index du sommet s dans le graphe g
    // la valeur UNKNOWN_INDEX si le sommet n'existe pas dans g)

    if(s >= ordre(g)){
      return UNKNOWN_INDEX;
    }

    return s;
}



// une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

void swap_sommets(arete *a){
  sommet temp = a->s2;
  a->s2 = a->s1;
  a->s1 = temp;
}

void normaliser_arete(arete *arete){
  if(arete->s2 < arete->s1){
    swap_sommets(arete);
  }
}


bool existe_arete(graphe const *g, arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes
    
    normaliser_arete(&a);
    for(size_t i=0; i<g->nb_aretes; i++){
      arete b = g->aretes[i];
      normaliser_arete(&b);
      if((a.s1 == b.s1 && a.s2 == b.s2)){
        return true;
      }
    }

    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    //  - les sommets s1 et s2 de a sont distincts
    //  - l'arête a n'existe pas dans g
    // /!\ si la capacité actuelle du tableau d'arêtes n'est pas suffisante,
    // /!\ il faut le réallouer.
    // /!\ on peut par exemple doubler la capacité du tableau actuel.

    // retourne true si l'arête a bien été ajoutée, false sinon

    normaliser_arete(&a);
    
    //Si les deux sommets de l'arête sont les mêmes
    if(a.s1 == a.s2){
      return false;
    }
    
    //Si les sommets n'existent pas dans g (s1 étant le plus petit, s'il n'est pas contenu s2 ne peut pas l'être)
    if(index_sommet(g, a.s1) == UNKNOWN_INDEX){
      return false;
    }

    //Si l'arête existe dans g
    if(existe_arete(g, a)){
      return false;
    }

    //Toutes les conditions sont réunies, alors on ajoute l'arête
    if(g->nb_aretes == g->aretes_capacite){
      arete* newTab = malloc(sizeof(arete) * g->aretes_capacite * 2);
      if(newTab == NULL){
        return false;
      }
      g->aretes_capacite *= 2;
      memcpy(newTab, g->aretes, sizeof(arete) * g->nb_aretes);  //OU REALLOC : malloc + copie + free    g->aretes = realloc(g->aretes, g->aretes_capacite*2); PUIS VERIFIER g->aretes != NULL
      free(g->aretes);
      g->aretes = newTab;
      g->aretes[g->nb_aretes] = a;
      g->nb_aretes++;
      return true;
    }

    g->aretes[g->nb_aretes] = a;
    g->nb_aretes++;
    return true;
}

size_t index_arete(graphe const *g, arete a)
{
    // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,
    // la valeur UNKNOWN_INDEX sinon

    normaliser_arete(&a);
    if(existe_arete(g, a)){
      for(size_t i=0; i<g->nb_aretes; i++){
        arete b = g->aretes[i];
        normaliser_arete(&b);
        if(a.s1 == b.s1 && a.s2 == b.s2){
          return i;
        }
      }
    }

    return UNKNOWN_INDEX;
}

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[])
{
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)

    size_t degre = 0;
    for(size_t i = 0; i<g->nb_aretes; i++){
      arete a = g->aretes[i];
      normaliser_arete(&a);
      if(a.s1 == s){
        sa[degre] = a.s2;
        degre++;
      }
      else if(a.s2 == s){
        sa[degre] = a.s1;
        degre++;
        // on peut écrire sa[degre++] = a.s1          le degré sera incrémenté après l'affectation
        // si on fait sa[++degre] = a.s1              l'incrémentation est faite avant
      }
    }

    return degre;
}
