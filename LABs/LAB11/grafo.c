#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "grafo.h"


typedef struct _viz Viz;
struct _viz {
  int noj;
  float peso;
  Viz*  prox;
};

struct _grafo {
  int nv;           /* numero de nos ou vertices */
  int na;           /* numero de arestas */
  Viz** viz;         /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

static Viz* criaViz(Viz* head, int noj, float peso) {
/* insere vizinho no inicio da lista */
  Viz* no = (Viz*) malloc(sizeof(Viz));
  assert(no);
  no->noj = noj;
  no->peso = peso;
  no->prox = head;
  return no;
}

static Grafo* grafoCria(int nv, int na) {

  int i;
  Grafo* g = (Grafo *) malloc(sizeof(Grafo));
  g->nv = nv;
  g->na = na;
  g->viz = (Viz **) malloc(sizeof(Viz *) * nv);
  for (i = 0; i < nv; i++)
    g->viz[i] = NULL;
  return g;
}


Grafo* grafoLe( char* filename ) {
/* cria grafo nÃ£o orientado - supÃµe que arquivo estÃ¡ correto! */

  FILE *arq = fopen(filename,"rt");
  int nv, na, no1, no2 = 0;
  float peso;
  Grafo* novo;
	
  fscanf(arq, "%d %d", &nv, &na);
  novo = grafoCria(nv, na);
  assert(novo);
  while (fscanf(arq, "%d %d %f", &no1, &no2, &peso) == 3) {
    novo->viz[no1] = criaViz(novo->viz[no1], no2, peso);
    novo->viz[no2] = criaViz(novo->viz[no2], no1, peso);
  }
  return novo;
}

Grafo*   grafoLibera(Grafo* grafo) {
  if (grafo) {
    int i = 0;
    Viz* no,*aux;
    for (i = 0; i < grafo->nv; i++){
      no = grafo->viz[i];
      while (no){
        aux = no->prox;
        free(no);
        no = aux;
      }
    }
    free(grafo->viz);
    free(grafo);
  }
  return NULL;
}

void grafoMostra (char* title, Grafo * grafo) {
  int i;
  if (title)
    printf("%s", title);
  if (grafo) {
    printf("NV: %d, NA: %d\n", grafo->nv, grafo->na);
    for (i = 0; i < grafo->nv; i++){
    Viz* viz = grafo->viz[i];
    printf("[%d]->", i);
    while (viz) {
      printf("{%d, %g}->", viz->noj, viz->peso);
      viz = viz->prox;
    }
    printf("NULL\n");
    }
  }
}

int menordist( int* caminhos, int* visitados) {
  int minimo = INT_MAX; int nomin = -1;
  int i;
  for (i=0;caminhos[i]!=-1;i++)
    if (!visitados[i] && caminhos[i]<minimo) {
      nomin = i;
      minimo = caminhos[i];
    }
  return nomin;
}

int* menoresCaminhos (Grafo *grafo, int no_inicial){
  if (no_inicial >= grafo->nv) return NULL;
	#if 1
	int * visitados = (int*)malloc(sizeof(int)*grafo->nv);
	int * caminhos = (int*)malloc((sizeof(int)*grafo->nv)+1);
	int * cmc = (int*)malloc((sizeof(int)*grafo->nv)+1);

	for(int i = 0; i < grafo->nv; i++)
		visitados[i] = 0;
	for(int i = 0; i < grafo->nv; i++)
		caminhos[i] = INT_MAX;
	caminhos[no_inicial] = 0;
	caminhos[grafo->nv] = -1;
	cmc[grafo->nv] = -1;
	cmc[no_inicial] = no_inicial;

	int val = no_inicial;
	while(val!=-1){
		printf("val = %d\n",val);
		Viz* viz = grafo->viz[val];
		while (viz) {
			if((caminhos[val]+viz->peso) < caminhos[viz->noj]){
				caminhos[viz->noj] = caminhos[val]+viz->peso;
				cmc[viz->noj] = val;
			}
			viz = viz->prox;
		}
			visitados[val] = 1;
			val = menordist(caminhos,visitados);
	}
	#endif
	printf("Menores caminhos:\n");
	for(int i = 0; i < grafo->nv; i++)
		printf("Index:%d, Val:%d\n",i,caminhos[i]);
	printf("Camimnhos mais curtos:\n");
	for(int i = 0; i < grafo->nv; i++)
		printf("Index:%d, Val:%d\n",i,cmc[i]);
  free(visitados);
  free(caminhos);
  return cmc;
}


