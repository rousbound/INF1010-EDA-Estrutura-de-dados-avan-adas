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
    if (chave==-111) {
			printf("Achou 1? %d\n",busca(mapacand,1));	
			printf("Achou 2? %d\n",busca(mapacand,2));	
			printf("Achou 3? %d\n",busca(mapacand,3));	
			printf("Achou 4? %d\n",busca(mapacand,4));	
			printf("Achou 5? %d\n",busca(mapacand,5));	
			printf("Achou 6? %d\n",busca(mapacand,6));	
			printf("Achou 7? %d\n",busca(mapacand,7));	
			printf("Achou 8? %d\n",busca(mapacand,8));	
			printf("Achou 9? %d\n",busca(mapacand,9));	
			printf("Achou 9999? %d\n",busca(mapacand,9999));	

			break;
		}

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

  destroi(mapacand); 
  destroi(mapagab); 

  return 0;
}

                  

