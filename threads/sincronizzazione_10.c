/*Si realizzi un programma in C che crea due thread secondari

 Il primo thread incrementa una variabile intera conteggio ogni 3 secondi, stampando il suo
valore ad ogni incremento

 Il secondo thread aspetta che la variabile conteggio raggiunga il valore 10; quando ciò
accade, la rimette a 0 e stampa un messaggio; dopodiché ricomincia da capo.

 Il thread principale resta in attesa (infinita) che i due thread secondari terminino.

 NOTA BENE: questo non è un problema produttore/consumatore; ma anche in questo caso
abbiamo:

 l’accesso a una risorsa condivisa (la variabile conteggio) da parte dei due thread

 l’attesa di una condizione che dipende dal valore della risorsa condivisa, da parte del
secondo thread
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREAD 2
#define attesa_incremento 1
#define limite_incremento 10

typedef struct{
    int *conteggio;
    pthread_mutex_t *mutex;
    pthread_cond_t limite;
}Tstruct;

void *incremento(void*arg);
void *reset(void *arg);

int main(){
    Tstruct buff;
    buff.conteggio=(int*)malloc(sizeof(int));
    *buff.conteggio=0;
    pthread_mutex_t m;
    buff.mutex=&m;
    pthread_mutex_init(&m,NULL);
    pthread_cond_init(&buff.limite,NULL);
    
    pthread_t tid1,tid2;
    
    pthread_create(&tid1,NULL,incremento,&buff);
    pthread_create(&tid2,NULL,reset,&buff);
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    return 0;
}
/*Il primo thread incrementa una variabile intera conteggio ogni 3 secondi, stampando il suo
valore ad ogni incremento

 Il secondo thread aspetta che la variabile conteggio raggiunga il valore 10; quando ciò
accade, la rimette a 0 e stampa un messaggio; dopodiché ricomincia da capo.*/
void *incremento(void*arg){
    Tstruct*buff=(Tstruct*)arg;
    
    while(1){
        pthread_mutex_lock(buff->mutex);
        
        while(*buff->conteggio==limite_incremento){
            pthread_cond_wait(&buff->limite,buff->mutex);
        }
        
        printf("thread incremento -> buff->conteggio=%d\n",*buff->conteggio);
        (*buff->conteggio)++;
        
        pthread_mutex_unlock(buff->mutex);
        pthread_cond_signal(&buff->limite);
        sleep(attesa_incremento);
    }
}
void *reset(void *arg){
    Tstruct*buff=(Tstruct*)arg;
    
    while(1){
        pthread_mutex_lock(buff->mutex);
        
        while(*buff->conteggio<limite_incremento){
            pthread_cond_wait(&buff->limite,buff->mutex);
        }
        
        printf("thread reset   -> buff->conteggio=%d\n",*buff->conteggio);
        *buff->conteggio=0;
        
        pthread_mutex_unlock(buff->mutex);
        pthread_cond_signal(&buff->limite);
    }
}


