#include <stdio.h>
#include "cmapa.h"
#include "arvore.h"

int main (void) {

  int chave;

  Mapa *mapacand = cria();
  Mapa *mapagab = cria();

  while (1) {
    printf ("chave a inserir (-111 para terminar): ");
    scanf ("%d", &chave);
    if (chave==-111) break;

    mapacand = insere (mapacand, chave);
    mapagab = cinsere (mapagab, chave);

    printf("mapa aluno: \n");
    mostra (mapacand);
    printf("mapa gabarito: \n");
    mostra (mapagab);

    if (!ciguais(mapagab, mapacand)) {
      printf("erro!\n");
      break;
    }
    else {
      printf("ok!\n");
    }
  }

  while (1) {
    printf ("chave a retirar (-111 para terminar): ");
    scanf ("%d", &chave);
    if (chave==-111) break;

    mapacand = retira (mapacand, chave);
    mapagab = cretira (mapagab, chave);

    printf("mapa aluno: \n");
    mostra (mapacand);
    printf("mapa gabarito: \n");
    mostra (mapagab);

    if (!ciguais(mapagab, mapacand)) {
      printf("erro!\n");
      break;
    }
    else {
      printf("ok!\n");
    }
  }


  destroi(mapacand); 
  destroi(mapagab); 

  return 0;
}

                  

