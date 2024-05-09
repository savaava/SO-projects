#include <pthread.h>
#include <stdio.h>

#define THREADS 2

#define START1 1 
#define START2 51
#define END1 50
#define END2 100

typedef struct{
    int start;
    int end;
    int*psomma;
    pthread_mutex_t*mutex;
}TParam;

void*calcola_somma(void*);

int main(){
    int i,risultato=0;
    pthread_t tid[THREADS];
    TParam dati[THREADS];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    
    dati[0].start=START1;
    dati[0].end=END1;
    dati[0].psomma=&risultato;
    dati[0].mutex=&mutex;
    
    dati[1].start=START2;
    dati[1].end=END2;
    dati[1].psomma=&risultato;
    dati[1].mutex=&mutex;
    
    for(i=0; i<THREADS; i++){
        if(pthread_create(tid+i,NULL,calcola_somma,dati+i) !=0){
            fprintf(stderr,"Errore nella creazione di un thread\n");
            return 1;
        }
    }
    
    for(i=0; i<THREADS; i++)
        pthread_join(tid[i],NULL);
        
    printf("Il risultato della somma è:%d\n",risultato);
}

void*calcola_somma(void*x){
    TParam*dati=(TParam*)x;
    int i,s=0;
    
    for(i=dati->start; i<=dati->end; i++)
        s+=i;
    
    pthread_mutex_lock(dati->mutex);
    *(dati->psomma)+=s;
    pthread_mutex_unlock(dati->mutex);
}






