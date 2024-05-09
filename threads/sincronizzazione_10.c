/*Esercizio 10 (Sincronizzazione)
Scrivere un programma in C che crea due thread secondari Il primo thread incrementa una variabile
intera conteggio ogni 3 secondi, stampando il suo valore ad ogni incremento. Il secondo thread aspetta
che la variabile conteggio raggiunga il valore 10; quando accade, la rimette a 0 e stampa un messaggio;
dopodiché ricomincia da capo. Il thread principale resta in attesa (infinita) che i due thread secondari
terminino.
NOTA BENE: questo non è un problema produttore/consumatore; ma anche in questo caso abbiamo:
• l’accesso a una risorsa condivisa (la variabile conteggio) da parte dei due thread;
• l’attesa di una condizione che dipende dal valore della risorsa condivisa, da parte del secondo
thread.*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 2
#define ATTESA 3
#define LIMITE 10

typedef struct {
    int conteggio;
    pthread_mutex_t mutex;
    pthread_cond_t limite_raggiunto;
} TStrutturaConteggio;

void *incrementa_conteggio(void *arg);
void *controlla_limite(void *arg);

int main(){
    TStrutturaConteggio dato_condiviso;
    pthread_mutex_init(dato_condiviso.mutex, NULL);
    pthread_cond_init(dato_condiviso.limite_raggiunto, NULL);
    
    pthread_t tid[THREADS];
    
}











