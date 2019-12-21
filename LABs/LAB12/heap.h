typedef struct _heap Heap;

Heap *heap_cria (int tam);
/* cria nova lista de prioridades */

void heap_insere (Heap *h, int prioridade, int no1, int no2); 
/* insere um novo dado na lista */

int heap_remove(Heap *h, int* no1, int* no2);
/* remove e retorna a maior prioridade e os dois nÃ³s associados a ela */
/* retorna -1 caso a lista esteja vazia */

void heap_libera(Heap *h);
/* libera a lista de prioridades */

void debug_heap_show (Heap *h, char* title);
/* sÃ³ para debug: enxerga representaÃ§Ã£o interna! */




