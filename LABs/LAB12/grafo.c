#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "grafo.h"
#include "heap.h"
#include "ub.h"


typedef struct _viz Viz;
struct _viz {
  int noj;
  int peso;
  Viz*  prox;
};

struct _grafo {
  int nv;           /* numero de nos ou vertices */
  int na;           /* numero de arestas */
  Viz** viz;         /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

static Viz* criaViz(Viz* head, int noj, int peso) {
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
  assert(g);
  g->nv = nv;
  g->na = na;
  g->viz = (Viz **) malloc(sizeof(Viz *) * nv);
  assert(g->viz);
  for (i = 0; i < nv; i++)
    g->viz[i] = NULL;
  return g;
}


Grafo* grafoLe( char* filename ) {
/* cria grafo nÃ£o orientado - supÃµe que arquivo estÃ¡ correto! */

  FILE *arq = fopen(filename,"rt");
  int nv, na, no1, no2 = 0;
  int peso;
  Grafo* novo;
	
  fscanf(arq, "%d %d", &nv, &na);
  novo = grafoCria(nv, na);
  assert(novo);
  while (fscanf(arq, "%d %d %d", &no1, &no2, &peso) == 3) {
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

void grafoMostra (Grafo * grafo) {
  int i;
  if (grafo) {
    printf("NV: %d, NA: %d\n", grafo->nv, grafo->na);
    for (i = 0; i < grafo->nv; i++){
      Viz* viz = grafo->viz[i];
      printf("[%d]->", i);
      while (viz) {
        printf(" {%d, %d}", viz->noj, viz->peso);
        viz = viz->prox;
      }
      printf("\n");
    }
  }
}

Grafo* arvoreCustoMinimo (Grafo* g) {
	
	Heap * minEdges;	
	
	minEdges = heap_cria(g->na);
	
	int val = g->nv;
	while(val--){
		Viz * viz = g->viz[val];
		while(viz){
			if(val < viz->noj){
				heap_insere(minEdges,viz->peso,val,viz->noj);
				}
			viz = viz->prox;
			}
	}
	
	int ktam = g->nv-1;
	int atual;
	Grafo * kruskal = grafoCria(g->nv,ktam);
	int tam = g->nv;
	UniaoBusca * ub = ub_cria(tam);
	
	int i1,i2;	
	int resp1,resp2;

	while(ktam){
		atual = heap_remove(minEdges,&i1,&i2);
		resp1 = ub_busca(ub,i1);
		resp2 = ub_busca(ub,i2);
		printf("noj 1 e 2: %d, %d ;resp 1 e 2: %d, %d\n",i1,i2,resp1,resp2);
		if(resp1 != resp2){
			ub_uniao(ub,i1,i2);	
			ktam--;
			kruskal->viz[i1] = criaViz(kruskal->viz[i1], i2, atual);
			kruskal->viz[i2] = criaViz(kruskal->viz[i2], i1, atual);
		}
	}
	debug(ub);
	ub_libera(ub);
	heap_libera(minEdges);

  return kruskal;
}

