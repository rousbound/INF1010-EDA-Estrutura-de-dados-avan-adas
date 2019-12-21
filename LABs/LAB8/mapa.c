#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mapa.h"

#define TAMINICIAL 11

typedef struct {
  int chave;
  int dados;
  int prox;
} ttabpos;

struct smapa {
  int tam;
  int ocupadas;
  ttabpos *tabpos;
};

static unsigned int hash (Mapa* m, int a) {
  return a%(m->tam);
}
  
Mapa* cria () {
  int i;
  Mapa* m = (Mapa*) malloc (sizeof(Mapa));
  if (m==NULL) {printf("erro na alocaÃ§Ã£o! \n"); exit(1);}
  m->tabpos = (ttabpos*) malloc (TAMINICIAL*sizeof(ttabpos)); 
  if (m->tabpos==NULL) {printf("erro na alocaÃ§Ã£o! \n"); exit(1);}
  m->tam = TAMINICIAL;
  m->ocupadas = 0;
  for (i=0;i<TAMINICIAL;i++) {
    m->tabpos[i].chave = -1;
    m->tabpos[i].prox = -1;
  }
  return m;
}

static void redimensiona (Mapa* m) {
#if 1
  int i;
  int tamanterior = m->tam;
  ttabpos* anterior = m->tabpos;

  /* aumenta o tamanho */
  m->tam = 1.947*m->tam;

  m->tabpos = (ttabpos*) malloc (m->tam*sizeof(ttabpos));
	for(int i = 0; i < m->tam; i++)
		{
			m->ocupadas = 0;
			m->tabpos[i].chave = -1;
			m->tabpos[i].dados = -1;
		}
	for(int i = 0; i < tamanterior ; i++)
	{
		if(anterior[i].chave != -1)
			insere(m,anterior[i].chave,anterior[i].dados);
		
	}
	

  free (anterior);
#endif
}

/* inserÃ§Ã£o: supÃµe que a chave dada NÃƒO estÃ¡ no mapa */
Mapa* insere (Mapa* m, int chave, int dados) {

  if (m->ocupadas > 0.75*m->tam) redimensiona(m);

  int pos = hash(m, chave);

  if (m->tabpos[pos].chave == -1) { /* estÃ¡ vazia */
    m->tabpos[pos].chave = chave;
    m->tabpos[pos].dados = dados;
    m->tabpos[pos].prox = -1;
  }
  else { /* conflito */
    /* procura proxima posiÃ§Ã£o livre */
    int poslivre = pos;
    do 
      poslivre = (poslivre+1) % (m->tam);
    while ((poslivre!=pos) && (m->tabpos[poslivre].chave!=-1));

    if (poslivre==pos) { /* tabela cheia -- nÃ£o deveria acontecer */
      printf ("pÃ¢nico, tabela cheia!\n"); exit(1);
    }

    /* achou posicao livre  - verificar quem vai para ela */
    int hashocupadora = hash(m, m->tabpos[pos].chave);
    if (hashocupadora==pos) { /* conflito primario: encadeia */
			m->tabpos[poslivre].chave = chave;
			m->tabpos[poslivre].dados = dados;
			m->tabpos[poslivre].prox = m->tabpos[pos].prox; 
			m->tabpos[pos].prox = poslivre;
	
    }
    else { /* conflito secundario: expulsa o item atual de pos */

			m->tabpos[poslivre].chave = m->tabpos[pos].chave;
			m->tabpos[poslivre].dados = m->tabpos[pos].dados;
			m->tabpos[poslivre].prox = m->tabpos[pos].prox; 

			while(m->tabpos[hashocupadora].prox != pos) 
				hashocupadora = m->tabpos[hashocupadora].prox;
			m->tabpos[hashocupadora].prox = poslivre;

			m->tabpos[pos].chave = chave;
			m->tabpos[pos].dados = dados;
			m->tabpos[pos].prox = -1; 

	
    }
  }
  (m->ocupadas)++; /* aumentou o nÃºmero de itens na tabela */
  return m;
}

int busca (Mapa *m, int chave) {
  if (m==NULL) return -1;
  int pos = hash(m, chave);
	while(m->tabpos[pos].chave != chave)
		{
			pos = m->tabpos[pos].prox;
			if(m->tabpos[pos].chave == chave)
				return m->tabpos[pos].dados;
			else if( pos == -1)
				return -1;
		}
	return m->tabpos[pos].dados;
}

void destroi (Mapa *m) {
  free (m->tabpos);
  free (m);
}

Mapa * retira(Mapa *m, int chave)
{
  if (m==NULL) return NULL;
  (m->ocupadas)--;
  int pos = hash(m, chave);
	int anterior = -1;
	while(m->tabpos[pos].chave != chave)
		{
			anterior = pos;	
			pos = m->tabpos[pos].prox;
			if( pos == -1)
				{
					printf("Return map\n");
					return m;
				}
		}
	int prox = m->tabpos[pos].prox;
  if (prox != -1) {
    m->tabpos[pos].chave = m->tabpos[prox].chave;
    m->tabpos[pos].dados = m->tabpos[prox].dados;
    m->tabpos[pos].prox = m->tabpos[prox].prox;
		m->tabpos[prox].chave = -1;
		m->tabpos[prox].dados = -1;
		m->tabpos[prox].prox = -1;
    return m;
  }
	else
	{
    m->tabpos[pos].chave = -1;
    m->tabpos[pos].dados = -1;
    m->tabpos[pos].prox = -1;
	}
		
	if(anterior != -1){
	   m->tabpos[anterior].prox = m->tabpos[pos].prox;
     return m;
	}	
	else
	{
    m->tabpos[pos].chave = -1;
    m->tabpos[pos].dados = -1;
    m->tabpos[pos].prox = -1;
	}
		
			
	return m;
	
}



int iguais (Mapa* m1, Mapa* m2) {
  int i;

  if (m1==NULL || m2==NULL) return (m1==NULL && m2==NULL);
  if (m1->tam != m2->tam) return 0;
  ttabpos* tp1 = m1->tabpos;
  ttabpos* tp2 = m2->tabpos;
  for (i = 0; i < m1->tam; i++)
    if ((tp1[i].chave != tp2[i].chave) ||
        (tp1[i].prox != tp2[i].prox))
      return 0;
  return 1;
}

void mostra (Mapa* m) {
  int i;
  for (i=0;i<m->tam;i++) 
    if (m->tabpos[i].chave!=-1) 
      printf ("posicao %d, chave %d, proximo %d\n", i, m->tabpos[i].chave, m->tabpos[i].prox);
}

