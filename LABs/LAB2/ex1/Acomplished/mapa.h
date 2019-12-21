#ifndef MAPA_H
#define MAPA_H
#endif

#include <limits.h>

typedef struct smapa Mapa;

Mapa* cria (void);
Mapa* insere (Mapa* m, int chave, int dados);
int busca (Mapa *m, int chave);
Mapa* retira (Mapa *m, int chave);
void destroi (Mapa *m);

