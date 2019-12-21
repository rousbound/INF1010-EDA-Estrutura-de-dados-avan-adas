#include <stdio.h>
#include "mapa.h"
#include "arvore.h"

int main (void) {

  int chave;

  Mapa *mapacand = cria();

  while (1) {
    printf ("chave a inserir (-111 para terminar): ");
    scanf ("%d", &chave);
    if (chave==-111) break;

    mapacand = insere (mapacand, chave);

    printf("mapa aluno: \n");
    mostra (mapacand);
  }

  while (1) {
    printf ("chave a retirar  (-111 para terminar): ");
    scanf ("%d", &chave);
    if (chave==-111) break;

    mapacand = retira (mapacand, chave);

    printf("mapa aluno: \n");
    mostra (mapacand);
  }

  destroi(mapacand); 

  return 0;
}

                  

