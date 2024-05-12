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
    TStrutturaConteggio dato;
    dato.conteggio=0;
    pthread_mutex_init(&dato.mutex,NULL);
    pthread_cond_init(&dato.limite_raggiunto,NULL);
    
    pthread_t tid[THREADS];
    int i;
    
    if(pthread_create(tid, NULL, incrementa_conteggio, &dato) !=0){
        fprintf(stderr,"Errore nella creazione di un thread\n");
        return 1;
    }
    if(pthread_create(tid+1, NULL, controlla_limite, &dato) !=0){
        fprintf(stderr,"Errore nella creazione di un thread\n");
        return 1;
    }

    for(int i=0; i<THREADS; i++)
        pthread_join(tid[i],NULL); 
        //attesa infinita, perchè in questo modo finchè i thread non finiscono il main non riprende
        
}

void *incrementa_conteggio(void *arg){
    TStrutturaConteggio*dato=(TStrutturaConteggio*)arg;
    
    while(1){
        pthread_mutex_lock(&dato->mutex);
        
        //sezione critica
        dato->conteggio+=1;
        printf("Conteggio corrente= %d\n",dato->conteggio);
        
        pthread_cond_signal(&dato->limite_raggiunto);
        pthread_mutex_unlock(&dato->mutex);
        sleep(ATTESA);
    }
}
void *controlla_limite(void *arg){
    TStrutturaConteggio*dato=(TStrutturaConteggio*)arg;
    
    while(1){
        pthread_mutex_lock(&dato->mutex);
        
        while(dato->conteggio!=LIMITE)
            pthread_cond_wait(&dato->limite_raggiunto, &dato->mutex);
        
        dato->conteggio=0;
        printf("Conteggio rimessa a %d\n",dato->conteggio);
        
        pthread_cond_signal(&dato->limite_raggiunto);
        pthread_mutex_unlock(&dato->mutex);
    }
}









