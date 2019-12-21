#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mapa.h"
#include "arvore.h"

enum minhaposicao {ESQ, MEIO, DIR};
typedef enum minhaposicao tpos;

enum resultadoretirada {OK, RETIRA_MENOR, RETIRA_MAIOR};
typedef enum resultadoretirada tresultret;

struct smapa
{
	int  kp, kg;   /* chaves: kp<kg, se kg existir. Se kg=-1, significa que ele nÃ£o existe. */
	Mapa  *pai;
	Mapa  *esq;
	Mapa  *meio;
	Mapa  *dir;
};

Mapa* cria (void) {
  return NULL;
}

static Mapa* novoNo (int chave) {
  Mapa *m = (Mapa*) malloc(sizeof(struct smapa));
  if (m==NULL) { printf ("erro no malloc! \n"); exit(1);}
  m->pai = NULL;
  m->kp = chave; 
  m->kg = -1;
  m->esq = m->meio = m->dir = NULL;
  return m;
}

static Mapa* overflowQuebra (Mapa *m, int *valorainserir, Mapa* novofilho) {
  Mapa* novo;

  novo = (Mapa*) malloc(sizeof(struct smapa));

  if (novo==NULL) { printf ("erro em malloc! \n"); exit(1);}

  if (*valorainserir < m->kp) {
    novo->esq = novofilho;
    if (novo->esq) novo->esq->pai = novo;
    novo->kp = *valorainserir;
    novo->meio = m->esq;
    if (novo->meio) novo->meio->pai = novo;
    novo->kg = -1;
    novo->dir = NULL;
    *valorainserir = m->kp;
    m->esq = m->meio;
    m->kp = m->kg;
  }
  else if (*valorainserir < m->kg) {
    novo->esq = m->esq;
    if (novo->esq) novo->esq->pai = novo;
    novo->kp = m->kp;
    novo->meio = novofilho;
    if (novo->meio) novo->meio->pai = novo;
    novo->kg = -1;
    novo->dir = NULL;
    *valorainserir = *valorainserir;
    m->esq = m->meio;
    m->kp = m->kg;
  } 
  else {
    novo->esq = m->esq;
    if (novo->esq) novo->esq->pai = novo;
    novo->kp = m->kp;
    novo->meio = m->meio;
    if (novo->meio) novo->meio->pai = novo;
    novo->kg = -1;
    novo->dir = NULL;
    m->kp = *valorainserir;
    *valorainserir = m->kg;
    m->esq = novofilho;
    if (m->esq) m->esq->pai = m;
  }
  m->meio = m->dir;
  m->kg = -1;
  m->dir = NULL;
  return novo;
}

static int insere2 (Mapa* m, int chave, int* valorainserir, Mapa** novofilho) {

  int inseriraqui = 0; /* indica se deve inserir neste nÃ³ */

  if (m==NULL) {
    printf("erro! subÃ¡rvore nula! \n"); exit (1);
  }

  if (m->esq != NULL) { /* nÃ£o Ã© folha, sÃ³ insere neste nÃ³ se subir um valor  */
    if (chave < m->kp) {
      inseriraqui = insere2(m->esq, chave, valorainserir, novofilho); 
    }
    else if (((m->kg != -1) && (chave < m->kg)) || (m->kg == -1)) {
      /* ou estÃ¡ entre as duas chaves ou sÃ³ tem uma chave no nÃ³ */
      inseriraqui = insere2(m->meio, chave, valorainserir, novofilho);
    }
    else { /* chave > m->kg */
      inseriraqui = insere2(m->dir, chave, valorainserir, novofilho);
    }
  }
  else { /* este nÃ³ Ã© folha, tem que inserir nele de qq jeito */
    *valorainserir = chave;
    inseriraqui = 1;
    *novofilho = NULL;
  } 

  if (!inseriraqui) return 0; /* inserÃ§Ã£o jÃ¡ estÃ¡ completa */

  /* procura espaÃ§o no nÃ³ */

  if (m->kg==-1) {
    if (*valorainserir < m->kp) { /* empurra chave existente para a direita */
      m->kg = m->kp;
      m->dir = m->meio;
      m->meio = m->esq;
      m->kp = *valorainserir;
      m->esq = *novofilho;
      if (m->esq) m->esq->pai = m;
    }
    else { /* Ã© maior que a chave que jÃ¡ estÃ¡ lÃ¡ */
      m->kg = *valorainserir;
      m->dir = m->meio;
      m->meio = *novofilho;
      if (m->meio) m->meio->pai = m;
    }
    return 0; /* como havia espaÃ§o, nÃ£o sobem valores a serem inseridos */
  }

  *novofilho = overflowQuebra (m, valorainserir, *novofilho); 

  return 1; /* quando hÃ¡ quebra sempre sobe a mediana para nova inserÃ§Ã£o */ 
}

Mapa* insere (Mapa* m, int chave) {
  int valorquesubiu;
  Mapa* novofilho;
  Mapa* novaraiz;

  if (m==NULL) {
    m = novoNo (chave);
    m->pai = novoNo (-1);
  }
  else {
    if (insere2 (m, chave, &valorquesubiu, &novofilho)) {
      /* cria nova raiz */
      novaraiz = novoNo (valorquesubiu);
      novaraiz->pai = m->pai;
      novaraiz->esq = novofilho;
      novaraiz->esq->pai = novaraiz;
      novaraiz->meio = m;
      novaraiz->meio->pai = novaraiz;
      m = novaraiz;
    } 
  }

  return m;
}

int busca (Mapa *m, int chave) {
  if (m == NULL) return 0;
  if (chave == m->kp || chave == m->kg) return 1;
  if (chave < m->kp)
    return busca(m->esq, chave);
  if (((m->kg != -1) && chave < m->kg) || (m->kg == -1))
    return busca(m->meio, chave);
  else
    return busca(m->dir, chave);
}

static tpos minhaposnopai (Mapa* pai, Mapa* filho) {
  if (filho == pai->esq)
    return ESQ;
  else if (filho == pai->meio) return  MEIO;
  else if (filho == pai->dir) return DIR;
  else return -1;
}

static void preenche (Mapa *m, Mapa *nesq, int nkp, Mapa *nmeio, int nkg, Mapa *ndir) {
  m->esq = nesq;
  m->kp = nkp;
  m->meio = nmeio;
  m->kg = nkg;
  m->dir = ndir;
}

static int maisaesquerda (Mapa *m) {
  while (m->esq != NULL) m=m->esq;
  return m->kp;
}

static tresultret retirarec (Mapa *m, int chave) {

  tresultret res;
  tpos minhapos;
  Mapa *filhoqueficou, *irmao;

  if (m==NULL) {
    printf("erro! subÃ¡rvore nula! \n"); exit (1);
  }

  if (m->esq != NULL) { /* nÃ£o Ã© folha */
    if (chave < m->kp) {
      res = retirarec (m->esq, chave);
    }
    else if (m->kp == chave) { /* achou - troca por succ */
      m->kp = maisaesquerda (m->meio);
      res = retirarec (m->meio, m->kp);
    }
    else if (((m->kg != -1) && (chave < m->kg)) || (m->kg == -1)) {
      /* ou estÃ¡ entre as duas chaves ou sÃ³ tem uma chave no nÃ³ */
      res = retirarec(m->meio, chave);
    }
    else if (m->kg == chave) { /* achou - troca por succ */
      m->kg = maisaesquerda (m->dir);
      res = retirarec (m->dir, m->kg);
    }
    else { /* chave > m->kg */
      res = retirarec(m->dir, chave);
    }
    if (res==OK) return OK;
  }
  else { /* este nÃ³ Ã© folha, ou chave estÃ¡ aqui ou nÃ£o estÃ¡ na Ã¡rvore*/
    if (chave==m->kp) res = RETIRA_MENOR;
    else if (chave == m->kg) res = RETIRA_MAIOR;
    else /* chave nÃ£o estÃ¡ na Ã¡rvore!!! */
      return OK;
  }

  /* retirada */
  /* pode ser porque estamos em uma folha ou porque "caiu" uma das chaves */

  if (res == RETIRA_MAIOR) { /* caso mais simples */
    preenche (m, m->esq, m->kp, m->meio?m->meio:m->dir, -1, NULL);
    return OK;
  }

  /* RETIRAMENOR */ 
  if (m->kg != -1) {
  /* ainda vai ficar um no nÃ³, tb simples */
    preenche (m, m->esq?m->esq:m->meio, m->kg, m->dir, -1, NULL);
    return OK;
  }
  
  /* RETIRAMENOR: essa Ã© a Ãºnica chave! combinar ou distribuir */

  minhapos = minhaposnopai (m->pai, m);

  /* se ainda tiver algum filho pegÃ¡-lo para passar para outro */
  filhoqueficou = m->esq?m->esq:m->meio;

  if (minhapos == ESQ) {
    irmao = m->pai->meio;
    if (irmao->kg == -1) { /* combinar */
      preenche (irmao, filhoqueficou, m->pai->kp, irmao->esq, irmao->kp, irmao->meio);
      if (irmao->esq) irmao->esq->pai = irmao;
      m->pai->esq = NULL;
      free(m);
      res = RETIRA_MENOR;
    }
    else { /* irmÃ£o tem duas chaves, redistribuir  */
      preenche (m, filhoqueficou, m->pai->kp, irmao->esq, -1, NULL);      
      if (m->esq) m->esq->pai = m;
      if (m->meio) m->meio->pai = m;
      preenche (m->pai, m->pai->esq, irmao->kp, m->pai->meio, m->pai->kg, m->pai->dir);
      preenche (irmao, irmao->meio, irmao->kg, irmao->dir, -1, NULL);
      res = OK;
    }  
  }
  else if (minhapos == MEIO) {
    irmao = m->pai->esq;
    if (irmao->kg == -1) { /* combinar */
			preenche(irmao, irmao->esq, irmao->kp, irmao->meio, m->pai->kp, m->esq);
			if(irmao->dir) irmao->dir->pai = irmao;
			m->pai->meio = NULL;
			free(m);
			res = RETIRA_MENOR;
			
    }
    else { /* irmÃ£o tem duas chaves, redistribuir  */
			preenche(m,irmao->dir,m->pai->kp, m->meio, m->kg, m->dir);//-1,NULL
			if(m->esq) m->esq->pai = m;
			if(m->meio) m->meio->pai = m;
			preenche(m->pai,irmao, irmao->kg, m, m->pai->kg, m->pai->dir);
			preenche(irmao, irmao->esq, irmao->kp, irmao->meio, -1,NULL);
			res = OK;
    }
  }
  else if (minhapos == DIR) {
    irmao = m->pai->meio;
    if (irmao->kg == -1) { /* combinar */
      /* completar */
    }
    else { /* irmÃ£o tem duas chaves, redistribuir  */
      /* completar */
    } 
  }
  return res;
}
      

Mapa* retira (Mapa *m, int chave) {
  tresultret res;
  Mapa* novaraiz;

  
  res = retirarec (m, chave);

  if (res == RETIRA_MAIOR) { /* caso mais simples */
    preenche (m, m->esq, m->kp, m->meio?m->meio:m->dir, -1, NULL);
  }

  else if (res == RETIRA_MENOR) {

    if (m->kg != -1) {
    /* ainda vai ficar um no nÃ³, tb simples */
      preenche (m, m->esq?m->esq:m->meio, m->kg, m->dir, -1, NULL);
    }

    else {
      /* vai acabar com a raiz */
      /* um dos filhos virou nulo - pegar o outro */
      novaraiz = (m->esq)?m->esq:m->meio;
      novaraiz->pai = m->pai;
      free(m);
      m = novaraiz;
    }
  }

  return m;
}

void destroi (Mapa *m) {
  if (m!= NULL) {
    destroi (m->esq);
    destroi (m->meio);
    destroi (m->dir);
    free (m);
  }
}

static void mostra_int  (Mapa *m) {
  printf ("[");
  if (m!= NULL) {
    printf ("%d:%d->%d ", m->kp, m->kg, m->pai->kp);
    mostra_int (m->esq);
    mostra_int (m->meio);
    if (m->kg != -1) mostra_int(m->dir);
  }
  printf ("]");
}

void mostra  (Mapa *m) {
  mostra_int (m);
  printf("\n");
}

int iguais (Mapa *m1, Mapa *m2) {
  if (m1==NULL) return (m2==NULL);
  if (m2==NULL) return 0;

  return (m1->kp == m2->kp) &&
          (m1->kg == m2->kg) &&
          iguais (m1->esq, m2->esq) &&
          iguais (m1->meio, m2->meio) &&
          iguais (m1->dir, m2->dir) &&
          (m1->pai->kp == m2->pai->kp);
}


