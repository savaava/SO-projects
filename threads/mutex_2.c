#include <pthread.h>
#include <stdio.h>

#define PROD_THREADS 1
#define CONS_THREADS 2

#define BUF_SIZE 4

typedef struct{
    int elem[BUF_SIZE];
    int primo;
    int ultimo;
    int cont;
    pthread_mutex_t mutex;
    pthread_cond_t non_pieno;
    pthread_cond_t non_vuoto;
}TBuffer;

typedef struct{
    TBuffer*buffer;
    int id;
    int ritardo;
}TParam;

void*(void*);

int main(){
    //prepariamo il buffer condiviso! per tutti i thread:
    TBuffer buffer;
    buffer.primo=0;
    buffer.ultimo=0;
    buffer.cont=0;
    pthread_mutex_init(&buffer.mutex,NULL);
    pthread_cond_init(&buffer.non_pieno,NULL);
    pthread_cond_init(&buffer.non_vuoto,NULL);
    
    int i;
    
    //tanti threads tanti parametri:
    TParam prod_param[PROD_THREADS];
    TParam cons_param[CONS_THREADS];    
    
    pthread_t prod_tid[PROD_THREADS];
    pthread_t cons_tid[CONS_THREADS];
    
    for(i=0; i<PROD_THREADS; i++){
        prod_param[i].id=i+1;
        prod_param[i].ritardo=2;
        prod_param[i].buffer=&buffer;
    }
    for(i=0; i<CONS_THREADS; i++){
        
    }
    
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

void*(void*x){

}




//la signal sulla condizione sblocca il thread bloccato su quella variabile cond

