/*Il candidato completi il programma fornito, completando la definizione delle strutture dati, il main e la funzione di
esecuzione dei thread denominata 'thread_execute'.

 Il programma ottiene un numero intero N dai parametri della linea di comando, e crea N thread, a ciascuno dei quali
passa un numero progressivo da 0 a N-1 (il primo thread creato riceve il numero 0, il secondo il numero 1, il terzo il
numero 2 e così via).

 I thread e il main comunicano attraverso un buffer che può contenere un singolo numero intero. Ciascun thread svolge
le seguenti operazioni:

 1. Aspetta che il buffer sia vuoto

 2. Scrive il proprio numero progressivo nel buffer, rendendolo pieno.

 3. Termina la sua esecuzione.

 Il main, dopo aver creato gli N thread, esegue il seguente algoritmo:

 1. Ripeti per N volte i seguenti passi:
  i) Aspetta che il buffer sia pieno
  ii) Preleva un numero dal buffer e stampa il numero, rendendo il buffer vuoto.
  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
    int n;
    int capacità;
    pthread_mutex_t mutex;
    pthread_cond_t pieno;
    pthread_cond_t vuoto;
}TBuffer;

typedef struct{
    TBuffer*buffer;
    int numero;
}Tparam;

void *thread_execute(void *arg);

int main(int argc, char*argv[]){
    if(argc!=2){
        printf("Non è stato inserito solo N dalla linea di comando\n");
        return 1;
    }
    
    int i;
    int N_THREADS=atoi(argv[1]);
    if(N_THREADS<1){
        printf("Non è stato inserito N>0 dalla linea di comando\n");
        return 1;
    }
    
    TBuffer buff;
    buff.capacità=0;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.pieno,NULL);
    pthread_cond_init(&buff.vuoto,NULL);
    
    Tparam param[N_THREADS];
    for(i=0; i<N_THREADS; i++){
        param[i].buffer=&buff;
        param[i].numero=i;
    }
    
    pthread_t tid[N_THREADS];
    for(i=0; i<N_THREADS; i++){
        if(pthread_create(tid+i, NULL, thread_execute, param+i) !=0){
            printf("Err nella creazione di un thread");
            return 1;
        }
    }
    
    int cont=N_THREADS;
    while(cont>0){
        cont--;
        pthread_mutex_lock(&buff.mutex);
        
        while(buff.capacità==0){
            pthread_cond_wait(&buff.pieno,&buff.mutex);
        }
        
        printf("MAIN -> prelevo %d\n",buff.n);
        buff.capacità=0; //ora il buff è vuoto
        
        pthread_mutex_unlock(&buff.mutex);
        pthread_cond_signal(&buff.vuoto);
    }
    
    return 0;
}

void *thread_execute(void *arg){
    Tparam*param=(Tparam*)arg;
    int n_prodotto=param->numero;
    TBuffer*buff=param->buffer;
    
    pthread_mutex_lock(&buff->mutex);
    
    while(buff->capacità==1){
        pthread_cond_wait(&buff->vuoto,&buff->mutex);
    }
    
    printf("THREAD -> produco %d\n",n_prodotto);
    buff->n=n_prodotto;
    buff->capacità=1; //ora il buff è pieno
    
    pthread_mutex_unlock(&buff->mutex);
    pthread_cond_signal(&buff->pieno);
    
    return NULL; //può terminare il thread
}









