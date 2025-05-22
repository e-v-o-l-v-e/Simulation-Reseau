#include "graphe.h"
#include "algos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    graphe g;
    init_graphe(&g);

    generer_complet(&g, 5);

    printf("AFFICHE GRAPHE\n");
    printf("==============\n");
    afficher(&g);

    if (est_regulier(&g))
      printf("Le graphe est régulier\n");
    else
      printf("Le graphe n'est pas régulier\n");

    deinit_graphe(&g);

    return EXIT_SUCCESS;
}
