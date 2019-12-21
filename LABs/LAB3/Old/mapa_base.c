#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include "arvore.h"

struct smapa {
  int chave;
  int conteudo;
  short int fb;
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
    m->fb = 0;
  }
  return m;
}

Mapa* cria() {
  return NULL;
}

static Mapa* insere2(Mapa *m, int chave, int novodado, int *cresceu) {
  if (m == NULL) {
    m = cria_no(chave, novodado);
    *cresceu = 1;
  }
  else if (chave < m->chave) {
    m->esq = insere2(m->esq, chave, novodado, cresceu);
    if(*cresceu) {
      switch (m->fb) {
      case 1: /* antes hd > he, depois hd = he pois he aumentou */
        m->fb = 0;
        *cresceu = 0;
        break;
      case 0: /* antes hd = he, agora he > hd, avisa para cima que cresceu */
        m->fb = -1;
        break;
      case -1: /* antes hd < he, depois hd-he = -2 pois he aumentou! */
        m = corrige_esq(m);
        *cresceu = 0;
        break;
      }
    }

  }
  else if (chave > m->chave) {
    /* COMPLETAR */
  }
  return m;
}

static Mapa* rotaciona_dir(Mapa *m) {
  Mapa *esq = m->esq;  /* nova raiz */
  m->esq = esq->dir;
  esq->dir = m;
  return esq;
}

static Mapa* rotaciona_esq(Mapa *m) {
  /* FAZER */
  return NULL; /* sÃ³ estÃ¡ aqui para compilar direito */
}

static Mapa* corrige_esq(Mapa *m) {
  /* se esq tem fb -1 rotaciono para a direita */
  if (m->esq->fb == -1) {
    /* altura da sad da raiz Ã© h, e de sua sae ficou h+2 */
    /* movendo a raiz para a direita, suas duas sub-arvores ficam com altura h */
    /* movendo esq para posiÃ§Ã£o de raiz, suas duas sub-arvores ficam com altura h+1 */ 
    m->fb = m->esq->fb = 0; 
    return rotaciona_dir(m);
  }
  else/* if (m->esq->fb == 1) */ {
    /* serÃ¡ uma rotaÃ§Ã£o dupla esq-dir */
    /* serÃ£o modificados os fb de m, de m->esq e de m->esq->dir */   
    /* isso vai depender do fb de m->esq->dir */
    Mapa *n = m->esq->dir;

    switch (n->fb) {
    case -1: 
      m->fb = 1;
      m->esq->fb = n->fb = 0;
      break;
    case 0:
      m->fb = m->esq->fb = n->fb = 0;
      break;
    case 1:
      m->fb = n->fb = 0;
      m->esq->fb = -1;
      break;
    }
              
    /* COMPLETAR com as rotaÃ§Ãµes necessÃ¡rias */
    return NULL; /* sÃ³ estÃ¡ aqui para compilar direito */
  }
}

static Mapa* corrige_dir(Mapa *m) {
  /* FAZER */
  return NULL; /* sÃ³ estÃ¡ aqui para compilar direito */
}

Mapa* insere (Mapa* m, int chave, int novodado) {
  int cresceu = 0;
  return insere2(m, chave, novodado, &cresceu);
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

int altura (Mapa* m) {
  if (m==NULL) return -1;
  return ((m->fb==-1)?(altura(m->esq)):altura(m->dir))+1;
}

void mostra(Mapa* m) {
  printf("[");
  if (m != NULL) {
    printf("<%d:%d:%d> ", m->chave, m->fb, m->conteudo);
    mostra(m->esq);
    mostra(m->dir);
  }
  printf("] ");
}

