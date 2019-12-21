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

/* implementaÃ§Ã£o do MIN HEAP */
typedef struct heap Heap;
typedef struct _item Item;

struct _item {
  int dist;
  int idno;
};

struct heap {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  Item *itens; /* vetor de itens */
  int* posnos;
}; 

static Heap *heap_cria (int max) {
  int i;
  Heap* heap=(Heap*)malloc(sizeof(struct heap));
  heap->max=max;
  heap->pos=0;
  heap->itens = (Item *)malloc(max*sizeof(struct _item));
  heap->posnos = (int *)malloc(max*sizeof(int));
  for (i = 0; i < max; i++) heap->posnos[i] = -1;
  return heap;
}

static void heap_libera(Heap *h) {
  free(h->itens);
  free(h->posnos);
  free(h);
}

static void heap_insere (Heap *h, int distancia, int idno) {
  if (h->pos >= h->max) {
    printf("Heap CHEIO!\n");
    exit(1);
  }
  h->itens[h->pos].dist = distancia;
  h->itens[h->pos].idno = idno;
  h->posnos[idno] = h->pos;
  h->pos++;
 } 

static void troca(Heap *h, int a, int b) {
  int ida = h->itens[a].idno;
  int idb = h->itens[b].idno;
	Item f = h->itens[a];
  h->itens[a] = h->itens[b];
  h->itens[b] = f;
  h->posnos[ida] = b;
  h->posnos[idb] = a;
}

static void corrige_abaixo(Heap *h, int atual) {

	int pai=atual;
  int filho_esq, filho_dir, filho;
  while (2*pai+1 < h->max){
    filho_esq=2*pai+1;
    filho_dir=2*pai+2;
    if (filho_dir >= h->max) filho_dir=filho_esq;
    if (h->itens[filho_esq].dist < h->itens[filho_dir].dist)
        filho=filho_esq;
    else
        filho=filho_dir;
    if (h->itens[pai].dist > h->itens[filho].dist)
				troca(h,pai,filho);
    else
				break;
    pai=filho;
  }
}

static int heap_remove(Heap *h) {
  int idno;
  if (h->pos == 0) return -1;
  idno = h->itens[0].idno;
  h->posnos[idno] = -1;

  h->itens[0].idno = h->itens[h->pos-1].idno;
  h->itens[0].dist = h->itens[h->pos-1].dist;
  h->posnos[h->itens[0].idno] = 0;

  h->pos--;
  corrige_abaixo(h, 0);

  return idno;
}
  
static void debug_heap_show (Heap *h, char* title) {
  int i;
  printf("%s={",title);
  for (i=0; i<(h->pos); i++)
    printf("[%d , %d] ",h->itens[i].idno, h->itens[i].dist);
  printf("}\n");
}

static void heap_corrige (Heap* h, int novadist, int idno) {
	int pos = h->posnos[idno];
	h->itens[pos].dist = novadist;
	for(int i = pos; i >= 0;i--){
		Item pai = h->itens[(i-1)/2];
		int pai_pos = (i-1)/2;
		int filho_pos = i;
		Item filho = h->itens[i];
		if(filho.dist < pai.dist)
			troca(h,pai_pos,filho_pos);
	}
}


static Viz* criaViz(Viz* head, int noj, float peso) {
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

int* menoresCaminhos (Grafo *grafo, int no_inicial){
  if (no_inicial >= grafo->nv) return NULL;
	#if 1
	int * visitados = (int*)malloc(sizeof(int)*grafo->nv);
  int * caminhos = (int*)malloc((sizeof(int)*grafo->nv)+1);
  int * cmc = (int*)malloc((sizeof(int)*grafo->nv)+1);//Caminhos mais curtos

  for(int i = 0; i < grafo->nv; i++)
    visitados[i] = 0;
  for(int i = 0; i < grafo->nv; i++)
    caminhos[i] = INT_MAX;

  caminhos[no_inicial] = 0;
  caminhos[grafo->nv] = -1; 
  cmc[grafo->nv] = -1; 
  cmc[no_inicial] = no_inicial;

	Heap * h = heap_cria(grafo->nv);
	for(int i = 0; i < grafo->nv; i++){
    if (i != no_inicial)
		  heap_insere(h,caminhos[i],i);
	}
  int val = no_inicial;
  while(val!=-1){
    printf("val = %d\n",val);
    Viz* viz = grafo->viz[val];
    while (viz) {
			int pesao = caminhos[val]+viz->peso;
      if((pesao) < caminhos[viz->noj]){
        caminhos[viz->noj] = pesao;
        cmc[viz->noj] = val;
				//printf("before heap_corrige\n");
				heap_corrige(h,pesao,viz->noj);
      }   
      viz = viz->prox;
    }   
      visitados[val] = 1;
      val = heap_remove(h);
  }
  #endif
  free(visitados);
  free(caminhos);	
  heap_libera(h);
	

  return cmc;
}


