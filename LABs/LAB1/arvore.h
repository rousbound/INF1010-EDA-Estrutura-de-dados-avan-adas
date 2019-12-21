typedef struct smapa Mapa;

Mapa* cria_raiz(int chave, int dados, Mapa* sae, Mapa* sad);
void mostra (Mapa *m);
int num_nos (Mapa *m);
int maior_chave (Mapa *m);
int num_maiores_que (Mapa *m, int n);

