typedef struct smapa Mapa;

void mostra (Mapa *m);
int num_nos (Mapa *m);
int maior_chave (Mapa *m);
int num_maiores_que (Mapa *m, int n);

int altura (Mapa *m);
int e_balanceada (Mapa *m);

Mapa *balancear(Mapa *m);

