/*Per ogni numero naturale n si definisce n-mo numero armonico alternato la somma 
h_n=1 - 1/2 + 1/3-1/4 +... (-1)^n+1 1/n
la funzione fornita calcola il numero armonico alternato corrispondente
al numero naturale n.

Il candidato completi il programma fornito, completando
il main ed implementando le funzioni producer_thread e consumer_thread.
il main crea due thread, eseguono le due funzioni citate.
I due  thread condividono un buffer, definito attraverso la struttura TBuffer 
che può contenere un numero oppure essere vuoto.

Il thread producer ripete le seguenti operazioni:
1 aspetta che il buffer sia vuoto
2 legge un numero naturale da tastiera con scanf inserendolo nel buffer
3 rende il buffer non vuoto
4 ricomincia da 1 

Il thread consumer ripete le seguenti operazioni:
1 aspetta che il buffer sia non vuoto
2 calcola e stampa il numero armonico del numero nel buffer
3 rende il buffer vuoto
4 ricomincia da 1

l'inserimento di un numero minore o uguale a 0 termina il programma

esempio:
l'utente inserisce i numeri: 7 3 1 -1
Producer: inserisci un numero: 7
Consumer: numero alternato di 7 = 0.759524
Producer: inserisci un numero: 3
Consumer:
Producer:inserisci un numero: 1
Consumer:
Producer:inserisci un numero: -1
Consumer:
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int number;
    bool vuoto;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void *producer_thread(void *);
void *consumer_thread(void *);

double HarmonicNumber(int n) {
    double harmonic_number = 0;
    for(int i=0;i<n;i++)
        if(i%2==0)
            harmonic_number += 1.0/(1+i);
        else
            harmonic_number -= 1.0/(i+1);
    return harmonic_number;
}

int main(){
    //INSERIRE QUI IL CODICE
    TBuffer buff;
    buff.vuoto=true;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.cond,NULL);
    
    pthread_t tid_p, tid_c;
    
    if( pthread_create(&tid_p, NULL, producer_thread, &buff) !=0){
        printf("Errore nella creazione del thread");
        return 2;
    }
    if( pthread_create(&tid_c, NULL, consumer_thread, &buff) !=0){
        printf("Errore nella creazione del thread");
        return 2;
    }
    
    pthread_join(tid_c,NULL);
    
    return 0;
}

void *producer_thread(void *param) {
    /* INSERIRE IL CODICE */
    TBuffer*buff=(TBuffer*)param;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->vuoto==false){
            pthread_cond_wait(&buff->cond, &buff->mutex);
        }
        
        printf("Producer: inserisci un numero: ");
        scanf("%d",&buff->number);
        buff->vuoto=false;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_signal(&buff->cond);
    }
    
    return NULL;
}

void *consumer_thread(void *param) {
    /* INSERIRE IL CODICE */
    TBuffer*buff=(TBuffer*)param;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->vuoto==true){
            pthread_cond_wait(&buff->cond, &buff->mutex);
        }
        
        if(buff->number >1){
            printf("Consumer: numero alternato di %d = %f\n",buff->number, HarmonicNumber(buff->number));
            buff->vuoto=true;
        }else{
            printf("è stato inserito %d, terminazione...",buff->number);
            return NULL;
        }

        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_signal(&buff->cond);
    }
    
    return NULL;
}
