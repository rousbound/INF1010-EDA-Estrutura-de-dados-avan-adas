#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "arvore.h"
#include <time.h>


int dados (int chave) {
  /* inventa dados associados a uma chave */
  return 2*chave;
}

Mapa *preenche(Mapa *m, int inicio, int fim) {
  int meio;
  if(inicio > fim) return m;
  meio = (fim + inicio)/2;
  m = insere(m, meio, dados(meio));
  m = preenche(m, inicio, meio-1);
  m = preenche(m, meio+1, fim);
  return m;
}








int main(int argc, char **argv) {
  int res, i, tammapa;
	clock_t t_busca1, end_t1,total_t;
  if (argc!=2) {
    printf ("uso do programa: %s <tamanhomapa>\n", argv[0]);
    exit(0);
  }
  tammapa = atoi(argv[1]);
  Mapa *m1 = cria();
  Mapa *m2 = cria();

  /* preenchendo sem balancear */
  for (i=0;i<tammapa;i++) m1 = insere(m1, i, dados(i));
  printf("mapa desbalanceado -------------- \n");
  //mostra(m1);

  printf("\nbusca desbalanceada --------------\n");
	t_busca1 = clock(); 
  for (i=0; i<=(tammapa+1); i++) {
    res = busca(m1, i);
    //printf ("achou %d? %s\n", i, (res!=INT_MIN)?"sim":"nÃ£o");
  }
	end_t1 = clock();


	printf ("tempo: %f segundos da primeira busca:\n", ((double) (end_t1 - t_busca1)) /CLOCKS_PER_SEC);

  /* preenchendo balanceado */
  m2 = preenche (m2, 0, tammapa-1);
  printf("mapa balanceado ------------ \n");
  //mostra(m2);



  printf("\nbusca balanceada --------------\n");



	t_busca1 = clock();

	 for (i=0; i<=(tammapa+1); i++) {
    res = busca(m2, i);
    //printf ("achou %d? %s\n", i, (res!=INT_MIN)?"sim":"nÃ£o");
  }

	end_t1 = clock();

	
	printf ("tempo: %f segundos da segunda busca:\n", ((double) (end_t1- t_busca1)) /CLOCKS_PER_SEC);

	
	printf("Altura de m1: %d\n",altura(m1));
	printf("Altura de m2: %d\n",altura(m2));
	

	
	
  Mapa *m3 = cria();
	
	int m3_dados[6] = {3,1,2,4,5,6};
	

  for (i=0;i<6;i++) m3 = insere(m3, m3_dados[i], dados(m3_dados[i]));
	
	printf("Altura de m3: %d\n",altura(m3));
	
	printf("m1 é balanceada?: %s\n",e_balanceada(m1)? "Não": "Sim");
	printf("m2 é balanceada?: %s\n",e_balanceada(m2)? "Não": "Sim");
	printf("m3 é balanceada?: %s\n",e_balanceada(m3)? "Não": "Sim");
	

	m3 = balancear(m3);
	printf("m3 é balanceada?: %s\n",e_balanceada(m3)? "Não": "Sim");
	mostra(m3);
	printf("\n\n--------------\n\n");



	mostra(m1);	
	m1 = balancear(m1);
	printf("\n\n\n");
	mostra(m1);
	printf("m1 é balanceada?: %s\n",e_balanceada(m1)? "Não": "Sim");
	printf("Altura de m1: %d\n",altura(m1));
	


  destroi(m1);
  destroi(m2);
  destroi(m3);


  return 0;
}

