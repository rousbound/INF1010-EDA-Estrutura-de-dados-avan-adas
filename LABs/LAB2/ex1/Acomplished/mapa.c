#include <stdlib.h>
#include <stdio.h>

#include "mapa.h"
#include "arvore.h"

/* IMPLEMENTAÃ‡ÃƒO DE MAPA */

struct smapa {
  int chave;
  int dados;
  Mapa* esq;
  Mapa* dir;
};

/* FunÃ§Ãµes auxiliares */

static Mapa *cria_no (int c, int d) {
  Mapa *nn = (Mapa *)malloc(sizeof(Mapa));
  if (nn!=NULL) {
    nn->esq = nn->dir = NULL;
    nn->chave =c;
    nn->dados = d;
  }
  return nn;
}

/* FunÃ§Ãµes exportadas */

Mapa* cria (void) {
  return NULL;
}
int busca (Mapa *m, int chave) {
  while (m!=NULL) {
    if (chave < m->chave)
      m = m->esq;
    else if (chave > m->chave)
      m = m->dir;
    else
      return m->dados; /* achou */
  }
  return INT_MIN;
}  

void destroi (Mapa *m) {
  if (m==NULL) return;
  destroi (m->esq);
  destroi (m->dir);
  free(m);
}

Mapa *insere (Mapa *m, int chave, int d) {
  if (m==NULL) 
    return cria_no(chave, d);
  if (chave < m->chave)
    m->esq = insere(m->esq, chave, d);
  else
    m->dir = insere(m->dir, chave, d);
  return m;
}

Mapa *retira (Mapa *m, int chave) {
  Mapa *sucessor, *t;
  if (m==NULL) return NULL;
  if (chave < m->chave) m->esq = retira(m->esq, chave);
  else if (chave > m->chave) m->dir = retira(m->dir, chave);
  else { 
						if ((m->esq == NULL) && (m->dir == NULL)) { 
      free(m); m = NULL;
    }
    else if (m->esq == NULL)  { 
      t = m; m = m->dir; free(t);
    }
    else if (m->dir == NULL)  {
      t = m; m = m->esq; free(t);
    }
    else {
      sucessor = m->dir;
      while (sucessor->esq) sucessor = sucessor->esq;
      m->chave = sucessor->chave;
      m->dados = sucessor->dados;
      sucessor->chave = chave;
      m->dir = retira(m->dir, chave);
    }
  }
  return m;
}

/* IMPLEMENTAÃ‡ÃƒO DE ÃRVORE */

void mostra(Mapa* m) {
  printf("[");
  if (m != NULL) {
    printf("<%d %d> ", m->chave, m->dados);
    mostra(m->esq);
    mostra(m->dir);
  }
  printf("] ");
}

int num_nos (Mapa *m) {
  if (m == NULL) return 0;
  return num_nos(m->esq) + num_nos(m->dir) + 1;
}

int maior_chave (Mapa *m) {
  if (m == NULL) return INT_MIN;
  if (m->dir) return maior_chave(m->dir);
  return m->chave;
}
Mapa * menor_no (Mapa *m) {
  if (m == NULL) return m;
  if (m->esq) return menor_no(m->dir);
  return m;
}

int num_maiores_que (Mapa *m, int n) {
  int nos = 0;
  if (m == NULL) return 0;
  if (m->chave > n)
    nos += num_maiores_que(m->esq, n);
  nos += num_maiores_que(m->dir, n);
  if (m->chave > n)
    nos += 1;
  return nos;
}




/*int altura(Mapa * m)
{
	if(m==NULL)
		return -1;
	else if(altura(m->dir) > altura(m->esq))
		return altura(m->dir) +1;
	else
		return altura(m->esq) +1;


}*/


int altura(Mapa * m)
{

  if(m==NULL)
    return -1;
  else 
		{	
  		int lAltura = altura(m->esq);
  		int rAltura = altura(m->dir);
			if(lAltura > rAltura)		
        return lAltura +1;
      else
        return rAltura +1;
    }

}




int e_balanceada (Mapa *m) {
	if(m==NULL)
		return 0;
	else if( abs(altura( m->esq) -altura( m-> dir) ) > 1)
		return 1;
	else
		return e_balanceada(m->esq) + e_balanceada(m->dir); 
}




Mapa *balancear(Mapa *m) {
	int temp;
	if(m == NULL)
		return NULL;
	else if( abs(num_nos(m->dir) - num_nos(m->esq)) <= 1)
	{
		m->dir = balancear(m->dir);
		m->esq = balancear(m->esq);
		balancear(m->dir);
	}
	while(num_nos(m->dir) > (num_nos(m->esq) +1))
		{
			temp = m->chave;
			m = retira(m,temp);
			m = insere(m,temp,temp*2);
			m->dir = balancear(m->dir);
			m->esq = balancear(m->esq);
		}
	while(num_nos(m->esq) > (num_nos(m->dir) +1))
		{
			temp = m->chave;
			m = retira(m,temp);
			m = insere(m,temp,temp*2);
			m->dir = balancear(m->dir);
			m->esq = balancear(m->esq);
		}
	return m;
}

