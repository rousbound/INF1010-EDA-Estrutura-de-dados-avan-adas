#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include "arvore.h"
 



struct smapa {
  int chave;
  int conteudo;
  short int bf;
  struct smapa* esq;
  struct smapa* dir;
};


 
static Mapa* cria_no(int c, int novodado);
static Mapa* insere2(Mapa *m, int chave, int novodado, int *cresceu);
static Mapa* corrige_dir(Mapa *m);
static Mapa* corrige_esq(Mapa *m);
static Mapa* rotaciona_dir(Mapa *m);
static Mapa* rotaciona_esq(Mapa *m);


 
static Mapa* cria_no(int c, int novodado) {
  Mapa *m = (Mapa*)malloc(sizeof(Mapa));
  if (m != NULL) {
    m->esq = m->dir = NULL;
    m->chave = c;
    m->conteudo = novodado;
    m->bf = 0;
  }
  return m;
}


Mapa* cria() {
  return NULL;
}

 
Mapa* insere (Mapa* m, int chave, int novodado) {
  int cresceu = 0;
  return insere2(m, chave, novodado, &cresceu);
}

 
static Mapa* insere2(Mapa *m, int chave, int novodado, int *cresceu) {
  if (m == NULL) {
    m = cria_no(chave, novodado);
    *cresceu = 1;
  }
  else if (chave < m->chave) {
   m->esq = insere2(m->esq, chave, novodado, cresceu);
    if(*cresceu) {
      switch (m->bf) {
      case 1: 
        m->bf = 0;
        *cresceu = 0;
        break;
      case 0:
        m->bf = -1;
        break;
      case -1:
        m = corrige_esq(m);
        *cresceu = 0;
        break;
      }
  
  }
    }
  else if (chave > m->chave) {
    m->dir = insere2(m->dir, chave, novodado, cresceu);
    if(*cresceu) {
      switch (m->bf) {
      case 1: 
        m = corrige_dir(m);
        *cresceu = 0;
        break;
      case 0:
        m->bf = 1;
        break;
      case -1: 
        m->bf = 0;
        *cresceu = 0;
        break;
      }
    }
  }
  return m;
}

 
static Mapa* corrige_esq(Mapa *m) {
  if (m->esq->bf == -1) {
    m->bf = m->esq->bf = 0;
    return rotaciona_dir(m);
  }
  else if (m->esq->bf == 1) {
		Mapa *n = m->esq->dir;

    switch (n->bf) {
    case -1: 
      m->bf = 1;
      m->esq->bf = n->bf = 0;
      break;
    case 0:
      m->bf = m->esq->bf = n->bf = 0;
      break;
    case 1:
      m->bf = n->bf = 0;
      m->esq->bf = -1; 
      break;
    }   
		m->esq = rotaciona_esq(m->esq);
    return rotaciona_dir(m); 
  }
   
}

 
 
static Mapa* corrige_dir(Mapa *m) {
  if (m->dir->bf == 1) {
    m->bf = m->dir->bf = 0;
    return rotaciona_esq(m);
  }
  else if (m->dir->bf == -1) {
		Mapa *n = m->dir->esq;

    switch (n->bf) {
    case -1: 
      m->bf = n-> bf= 0;
      m->dir->bf = 1;
      break;
    case 0:
      m->bf = m->dir->bf = n->bf = 0;
      break;
    case 1:
      m->bf = -1; 
      m->dir->bf = n->bf = 0;
      break;
    }  
		m->dir = rotaciona_dir(m->dir); 
    return rotaciona_esq(m); 
  }
}


 
static Mapa* rotaciona_dir(Mapa *m) {
    Mapa* antesq = m->esq;
    m->esq = antesq->dir;
    antesq->dir = m; 
  return antesq; 
}

 
static Mapa* rotaciona_esq(Mapa *m) {
  Mapa* dir = m->dir;
  m->dir = dir->esq;
  dir->esq = m;
  return dir;
}

 
int busca (Mapa *m, int chave) {
  if (m == NULL) {
    return -1;
  }
  else if (m->chave == chave) {
    return m->conteudo;
  }
  else if (chave < m->chave) {
  return busca(m->esq, chave);
  }
  else if (chave > m->chave) {
    return busca(m->dir, chave);
  }
  else {
    return -1;
  }
}

 
 
void destroi (Mapa *m) {
  if (m != NULL) {
    destroi(m->esq);
    destroi(m->dir);
    free(m);
  }
}
 

 
int iguais (Mapa* m1, Mapa* m2) {
 
  if (m1==NULL) return (m2==NULL);
  if (m2==NULL) return 0;
 
  return (m1->chave == m2->chave) &&
          iguais (m1->esq, m2->esq) &&
          iguais (m1->dir, m2->dir);
}

 
int altura (Mapa* m) {
  if (m==NULL) return 0;
  printf ("chave %d\n", m->chave);
  return ((m->bf==-1)?(altura(m->esq)):altura(m->dir))+1;
}

 
static void mostra_mapa_int (Mapa* m) {
 
  printf("[");
  if (m != NULL) {
    printf("%d:%d", m->chave,m->bf);
    mostra_mapa_int(m->esq);
    mostra_mapa_int(m->dir);
  }
  printf("]");
}

 
void mostra (Mapa* m) {
  mostra_mapa_int (m); printf ("\n");
}


void print_crescente( Mapa* m,int min, int max){
	if(m==NULL)
		{
		return ;
		}
	else
{
	if(m->chave > min && m->chave < max)
		printf("%d ",m->chave);
	print_crescente(m->esq,min,max);
	print_crescente(m->dir,min,max);
}
}

void mostracaminholongo(Mapa * m)
{
	Mapa * temp = cria();
	temp = m;
	int i;
	while(temp!= NULL){
		if(temp->bf >0)
			temp=temp->dir;
		else
			temp=temp->esq;
		printf("%d ",temp->chave);
	}
	printf("\n");
}

	
