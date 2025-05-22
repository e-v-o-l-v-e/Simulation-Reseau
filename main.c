#include "network.h"
#include <stdio.h>
#define LINE_LENGHT 64

int main() {
  network n; 

  FILE *config;
  config = fopen("config1", "r");

  if (config == NULL) {
    perror("le fichier de configuration n'existe pas ou n'est pas lisible.");
    return 0;
  }

  char line[LINE_LENGHT];

  char *header = fgets(line, LINE_LENGHT, config);

  printf("%s", header);
}
