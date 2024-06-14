/******************************************************************************
Si realizzi un programma in C che implementi il problema produttore/consumatore con i threads
sincronizzati mediante mutex e variabili condition

 Produttori e consumatori hanno un proprio numero identificativo, assegnato in fase di
creazione

 Le informazioni scambiate tra produttori e consumatori sono numeri interi

 Il thread principale crea 1 produttore e 2 consumatori

 Il produttore produce numeri progressivi (1, 2, ...), aspettando un tempo prestabilito tra un
numero e l'altro

 Ciascun consumatore preleva i numeri dal buffer, li stampa a video e aspetta un tempo
prestabilito tra un numero e l’altro

 il massimo numero di informazioni da mantenere nel buffer è 4

 Il thread principale termina dopo 60 secondi
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PROD_THREAD 1
#define CONS_THREAD 2
#define ATTESA 60
#define DIM_BUFFER 4

typedef struct{
    int buffer[DIM_BUFFER];
    pthread_mutex_t mutex;
    pthread_cond_t non_vuoto;
    pthread_cond_t non_pieno;
    int primo, ultimo, lunghezza;
}TBuffer;

typedef struct{
    TBuffer *buffer;
    int id;
    int ritardo;
}Tstruct;

void *produttore(void *arg);
void *consumatore(void *arg);


int main(){
    int i;
    
    //inizializzazione buffer:
    TBuffer buff;
    buff.primo=0;
    buff.ultimo=0;
    buff.lunghezza=0;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.non_pieno,NULL);
    pthread_cond_init(&buff.non_vuoto,NULL);
    
    Tstruct param_cons[CONS_THREAD];
    Tstruct param_prod[PROD_THREAD];
    
    for(i=0; i<CONS_THREAD; i++){
        param_cons[i].id=i+1;
        param_cons[i].ritardo=3+2*i;
        param_cons[i].buffer=&buff;
    }
    for(i=0; i<PROD_THREAD; i++){
        param_prod[i].id=i+1;
        param_prod[i].ritardo=2;
        param_prod[i].buffer=&buff;
    }
    
    //creazione thread:
    pthread_t tid_cons[CONS_THREAD];
    pthread_t tid_prod[PROD_THREAD];
    
    for(i=0; i<CONS_THREAD; i++){
        if( pthread_create(tid_cons+i,NULL,consumatore,param_cons+i) !=0){
            printf("Errore nella creazione di un thread");
            return 2;
        }
    }
    for(i=0; i<PROD_THREAD; i++){
        if( pthread_create(tid_prod+i, NULL, produttore, param_prod+i) !=0){
            printf("Errore nella creazione di un thread");
            return 2;
        }
    }
    
    sleep(ATTESA);
    return 0;
}

void *produttore(void *arg){
    Tstruct*tmp=(Tstruct*)arg;
    int attesa=tmp->ritardo;
    int id=tmp->id;
    TBuffer*buff=tmp->buffer;
    int x=0;
    
    while(1){
        x++;
        
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->lunghezza==DIM_BUFFER){
            pthread_cond_wait(&buff->non_pieno, &buff->mutex);
        }
        
        buff->buffer[buff->ultimo]=x;
        buff->ultimo=(buff->ultimo+1)%DIM_BUFFER;
        buff->lunghezza++;
        
        printf("Produttore (id=%d) -> produce %d\n",id,x);
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_signal(&buff->non_vuoto);
        
        sleep(attesa);
    }
}
void *consumatore(void *arg){
    Tstruct*tmp=(Tstruct*)arg;
    int attesa=tmp->ritardo;
    int id=tmp->id;
    TBuffer*buff=tmp->buffer;
    int x;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->lunghezza==0){
            pthread_cond_wait(&buff->non_vuoto, &buff->mutex);
        }
        
        x=buff->buffer[buff->primo];
        buff->primo=(buff->primo+1)%DIM_BUFFER;
        buff->lunghezza--;
        
        printf("Consumatore (id=%d) -> preleva %d\n",id,x);
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_signal(&buff->non_pieno);
        
        sleep(attesa);
    }
}






