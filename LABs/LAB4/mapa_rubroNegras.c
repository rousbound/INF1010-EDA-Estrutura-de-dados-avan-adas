#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mapa.h"
#include "arvore.h"

#define TRUE 1

struct smapa {
  int chave;
  int dado;
  char vermelho;
  struct smapa* esq;
  struct smapa *dir;
};

typedef enum result {OK, RED, LEFTRED, RIGHTRED} Result;
static char *strstatus[] = {"ok", "red", "leftred", "rightred"};

static Mapa*cria_no (int c, int novodado);
static Mapa* rotaciona_dir(Mapa *m);
static Mapa* rotaciona_esq(Mapa *m);
static void trocaCores (Mapa *m);
static Mapa* corrigeDir (Mapa *m, Result* status);
static Mapa* corrigeEsq (Mapa *m, Result* status);
static Mapa* insereRec (Mapa* m, int chave, int novodado, Result* status);

static Mapa *cria_no (int c, int novodado) {
  Mapa *m = (Mapa *)malloc(sizeof(Mapa));
  if (m!=NULL) {
    m->esq = m->dir = NULL;
    m->chave =c;
    m->vermelho = TRUE;
    m->dado = novodado;
  }
  return m;
}

Mapa* cria (void) {
  return NULL;
}

void destroi (Mapa *m) {
  if (m!= NULL) {
    destroi (m->esq);
    destroi (m->dir);
    free(m);
  }
}

int busca (Mapa *m, int chave) {
  if (m == NULL) {
    return -1;
  }
  else if (m->chave == chave) {
    return m->dado;
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


Mapa* insere (Mapa* m, int chave, int novodado) {
  Result status;
  m = insereRec (m, chave, novodado, &status);
  if (status == RED) m->vermelho = 0;
  else if (status != OK) {
    printf ("erro ao voltar para a raiz: status invalido %s !\n", 
             strstatus[status]);
    mostra(m);
    exit(1);
  }
  return m;
}

void mostra (Mapa* m) {
  printf("[");
  if (m != NULL) {
    printf("<%d - %c> ", m->chave,(m->vermelho) ?  'r' : 'b');
    mostra(m->esq);
    mostra(m->dir);
  }
  printf("] ");
}

static Mapa* rotaciona_dir(Mapa *m) {
  Mapa* esq = m->esq;
  m->esq = esq->dir;
  esq->dir = m;
  return esq;
}

static Mapa* rotaciona_esq(Mapa *m) {
  Mapa* dir = m->dir;
  m->dir = dir->esq;
  dir->esq = m;
  return dir;
}

static void trocaCores (Mapa *m) {
  char corpai = m->vermelho; 
  m->vermelho = !corpai;     /* troca a cor do pai */
  m->esq->vermelho = corpai; /* os filhos recebem a cor inversa da do pai */
  m->dir->vermelho = corpai;
}

static Mapa* corrigeEsq (Mapa *m, Result* status) {
  switch (*status) {
    case OK:  /* nada a corrigir */
      break;
    case RED: /* filho vermelho */
      if (m->vermelho) *status = LEFTRED; /* nÃ³ vermelho, filho vermelho Ã  esquerda */
      else *status = OK; /* nÃ³ preto, filho vermelho estÃ¡ ok */
      break;
    case LEFTRED: /* LL */
      if (!m->dir || !m->dir->vermelho) {  /* filho direito preto: LLb */
        m = rotaciona_dir(m);
        trocaCores(m);
        *status = OK;
      }
      else { /* filho direito vermelho: LLr */
				trocaCores(m);
				*status = RED;
			
      }
      break;
    case RIGHTRED: /* LR */
      if (!m->dir || !m->dir->vermelho) {  /* filho direito preto: LRb */
        m->esq = rotaciona_esq(m->esq);
        m = rotaciona_dir(m);
        trocaCores(m);
        *status = OK;
      }
      else { /* filho direito vermelho -> LRr */
        trocaCores(m);
        *status = RED;
      }
      break;
  }
  return m;
}
          
static Mapa* corrigeDir (Mapa *m, Result* status) {
  switch (*status) {
    case OK: /* nada a corrigir */
      break;
    case RED: /* filho vermelho */
      if (m->vermelho) *status = RIGHTRED;
      else *status = OK;
      break;
    case RIGHTRED: /* RR */
      if (!m->esq || !m->esq->vermelho) { /* filho esquerdo preto: RRb */
				m = rotaciona_esq(m);
				trocaCores(m);
				*status = OK;
        /* completar */
      }
      else { /* filho esquerdo vermelho: RRr */
        /* completar */
				trocaCores(m);
				*status = RED;
      }
      break;
    case LEFTRED: /* RL */
      if (!m->esq || !m->esq->vermelho) { /* filho esquerdo preto: RLb */
				m->dir = rotaciona_dir(m->dir);
				m = rotaciona_esq(m);
				trocaCores(m);
				*status = OK;
        /* completar */
      }
      else { /* filho esquerdo vermelho: RLr */
				trocaCores(m);
				*status = RED;
        /* completar */
      }
      break;
  }
  return m;
}

static Mapa* insereRec (Mapa* m, int chave, int novodado, Result* status){
  if (m==NULL) {
    m = cria_no (chave, novodado);
    *status = RED;
  }
  else if (chave < m->chave) {
    m->esq = insereRec (m->esq, chave, novodado, status);
    m = corrigeEsq (m, status);
  }
  else if (chave > m->chave) {
    m->dir = insereRec (m->dir, chave, novodado, status);
    m = corrigeDir (m, status);
  }
  return m;
}

