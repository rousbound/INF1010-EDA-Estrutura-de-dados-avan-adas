#include <stdio.h>
#include "mapa.h"
#include "arvore.h"


int dados (int chave) {
  /* inventa dados associados a uma chave */
  return 2*chave;
}

int main() {
  int n;
  Mapa *m1 = cria_raiz (6,dados(6),
                        cria_raiz(2,dados(2),
                            cria_raiz(1,dados(1),cria(),cria()),
                            cria_raiz(4,dados(4),cria(),cria())),
                        cria_raiz(10,dados(10),
                            cria(),
                            cria_raiz(15,dados(15),cria(),cria())));
  Mapa *m2 = cria_raiz (7,dados(7),
                        cria_raiz(3,dados(3),
                            cria_raiz(1,dados(1),cria(),cria()),cria()),
                        cria_raiz(15,dados(15),
                            cria_raiz(12,dados(12),cria(),cria()),
                            cria_raiz(19,dados(19),
                                cria_raiz(18, dados(18),cria(),cria()),
                                cria())));


  /* Mostra a Ã¡rvore 1 */
	
	//m1 = insere(m1,5,dados(5));
  printf("Arvore 1:\n");
  mostra(m1);
  printf("\n");

  /* Mostra a Ã¡rvore 2 */
  printf("Arvore 2:\n");
  mostra(m2);
  printf("\n");

  /* ObtÃ©m o nÃºmero de nÃ³s da Ã¡rvore 1 */
  n = num_nos(m1);
  printf("NÃ³s da Ã¡rvore 1: %d -> %s\n",n,(n == 6)?"SIM":"NAO !!!");

  /* ObtÃ©m o nÃºmero de nÃ³s da Ã¡rvore 2 */
  n = num_nos(m2);
  printf("NÃ³s da Ã¡rvore 2: %d -> %s\n",n,(n == 7)?"SIM":"NAO !!!");

  /* ObtÃ©m a maior chave das Ã¡rvores */
  n = maior_chave(m1);
  printf("Maior chave da Ã¡rvore 1: %d -> %s\n",n,(n == 15)?"SIM":"NAO !!!");

  n = maior_chave(m2);
  printf("Maior chave da Ã¡rvore 2: %d -> %s\n",n,(n == 19)?"SIM":"NAO !!!");

  /* ObtÃ©m o nÃºmero de nÃ³s com chaves maiores que 3 */
  n = num_maiores_que(m1,3);
  printf("Chaves maiores que 3 Ã¡rvore 1: %d -> %s\n",n,(n == 4)?"SIM":"NAO !!!");

  n = num_maiores_que(m2,3);
  printf("Chaves maiores que 3 Ã¡rvore 2: %d -> %s\n",n,(n == 5)?"SIM":"NAO !!!");
	
  return 0;
}

