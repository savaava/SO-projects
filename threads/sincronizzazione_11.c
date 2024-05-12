/*Esercizio 11 (Sincronizzazione)
๏ Si realizzi un programma in C che crea un thread secondario che funge da produttore, mentre il
main funge da consumatore.
๏ Il buffer condiviso consente di memorizzare una sola stringa
๏ Il produttore legge una parola da tastiera e la inserisce nel buffer, poi ricomincia da capo.
๏ Il main, in un ciclo infinito, preleva la parola dal buffer e ne stampa la lunghezza*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PAROLA 100

void *produttore(void *arg);

typedef struct {
    char parola[MAX_PAROLA]; /* Contenuto del buffer */
    bool presente; /* Indica se c'è una parola */
    pthread_mutex_t mutex;
    pthread_cond_t cambiato;
} TBuffer;

int main(){
    TBuffer dati;
    pthread_mutex_init(&dati.mutex,NULL);
    pthread_cond_init(&dati.cambiato,NULL);
    dati.presente=false;
    
    pthread_t tid;
    if(pthread_create(&tid, NULL, produttore, &dati) !=0){
        fprintf(stderr,"Errore nella creazione del thread produttore\n");
        return 1;
    }
    
    while(1){
        pthread_mutex_lock(&dati.mutex);
        
        while(dati.presente==false)
            pthread_cond_wait(&dati.cambiato, &dati.mutex);
        
        //sezione critica
        printf("La lunghezza della stringa %s, è: %ld\n", dati.parola, strlen(dati.parola));
        dati.presente=false;
        
        pthread_cond_signal(&dati.cambiato);
        pthread_mutex_unlock(&dati.mutex);
    }
}

void *produttore(void *arg){
    TBuffer*dati=(TBuffer*)arg;
    
    while(1){
        pthread_mutex_lock(&dati->mutex);
        
        while(dati->presente==true)
            pthread_cond_wait(&dati->cambiato, &dati->mutex);
        
        //sezione critica
        printf("Inserire una nuova stringa da tastiera: ");
        scanf("%s",dati->parola);
        dati->presente=true;
        
        pthread_cond_signal(&dati->cambiato);
        pthread_mutex_unlock(&dati->mutex);
    }
}






