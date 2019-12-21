#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

/* aux para fila e pilha */
typedef struct _sq SQ;
struct _sq {
  int info;
  SQ* prox;
};

static SQ* newList (){
  return NULL;
}

static SQ* enqueue(SQ* queue, int info){
  SQ* novo = (SQ*)malloc(sizeof(SQ));
  SQ* aux = queue;
  assert(novo);
  novo->info = info;
  novo->prox = NULL;
  if (!queue){
    return novo;
  }
  while (aux->prox){
    aux = aux->prox;
  }
  aux->prox = novo;
  return queue;
}

static SQ* dequeue(SQ* queue,int* info){
  SQ* libera;
  if (!queue){
    *info = -1;
    return NULL;
  }
  *info = queue->info;
  libera = queue;
  queue = queue->prox;
  free(libera);
  return queue;
}

static SQ* pop(SQ* stack,int* popped_info){
  SQ* libera;
  if (!stack){
    *popped_info = -1;
    return NULL;
  }
  *popped_info = stack->info;
  libera = stack;
  stack = stack->prox;
  free(libera);
  return stack;
}

static SQ* push(SQ* stack, int info){
  SQ* novo = (SQ*)malloc(sizeof(SQ));
  assert(novo);
  novo->info = info;
  novo->prox = stack;
  return novo;
}

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

  FILE *arq = fopen(filename,"r");
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

Grafo*  grafoLibera(Grafo* grafo) {
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


void DFS(Viz** lista, int * visitados, int no_inicial){
	
	Viz * temp;
	temp = lista[no_inicial];

	visitados[no_inicial] = 1;	
	int no_prox = no_inicial;
	while(temp != NULL){
		no_prox = temp->noj;
		if (!visitados[no_prox]){
			printf("%d ",no_prox);
			DFS(lista,visitados,no_prox);
		}
		temp = temp->prox;
	}
}



void grafoPercorreProfundidade(Grafo *grafo, int no_inicial){
	
	int * visitados  = (int *)malloc(sizeof(int)*grafo->nv);

	for(int i = 0; i < grafo->nv; i++)
		visitados[i] = 0;
	
	DFS(grafo->viz,visitados,no_inicial);
	printf("End of grafo\n");
	
	free(visitados);
	
  printf ("\n");
} 

void grafoPercorreLargura(Grafo *grafo, int no_inicial){
	#if 1
	int * emfila  = (int *)malloc(sizeof(int)*grafo->nv);

	for(int i = 0; i < grafo->nv; i++)
		emfila[i] = 0;

	SQ* new = newList();
		
	Viz * temp;

	for(int i = 0; i < grafo->nv; i++){
		temp = grafo->viz[no_inicial];
		int no_prox = no_inicial;
		emfila[no_inicial] = 1;	
		while(temp != NULL){
			no_prox = temp->noj;
			if (!emfila[no_prox]){
				printf("%d ",no_prox);
				new = enqueue(new,no_prox);
				emfila[no_prox] = 1;	
			}
			temp = temp->prox;
		}
			new = dequeue(new,&no_inicial);
	}
	
	#endif
	free(emfila);
		

 	printf ("\n");
} 

void grafoPercorreProfundidade2 (Grafo *grafo, int no_inicial){

	int * visitados  = (int *)malloc(sizeof(int)*grafo->nv);

	for(int i = 0; i < grafo->nv; i++)
		visitados[i] = 0;
	
	SQ * stack = newList();

	Viz * temp;

	stack = push(stack,no_inicial);
	while(stack){
		temp = grafo->viz[no_inicial];
		int no_prox = no_inicial;
		visitados[no_inicial] = 1;
		while(temp!=NULL){
			no_prox = temp->noj;
			if(!visitados[no_prox]){
				stack = push(stack,no_prox);
				visitados[no_prox] = 1;
				}
			temp = temp->prox;
		}
		printf("%d ", no_inicial);
		stack = pop(stack,&no_inicial);
		temp = grafo->viz[no_inicial];
	}
		

 printf ("\n");
} 


