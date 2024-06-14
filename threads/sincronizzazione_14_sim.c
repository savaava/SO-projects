/*๏ Si realizzi un programma in C che implementi il problema produttore/consumatore con i threads
sincronizzati mediante mutex con variabili condition.

๏ Il programma gestisce un buffer che contiene una singola parola (una stringa); il buffer può essere
vuoto (non è presente la stringa) oppure pieno (è presente la stringa); non è prevista la possibilità
che il buffer contenga più di una stringa.

๏ Un thread Produttore aspetta che il buffer sia vuoto; quindi legge da tastiera una parola e la inserisce
nel buffer (che così diventa pieno); quindi il Produttore ricomincia da capo

๏ Un thread Consumatore1 aspetta che il buffer sia pieno e contenga una parola che comincia per una
vocale; in tal caso il Consumatore1 stampa la parola a video, svuota il buffer e ricomincia da capo

๏ Un thread Consumatore2 aspetta che il buffer sia pieno e contenga una parola che non comincia per
una vocale; in tal caso il Consumatore2 stampa a video un messaggio che indica la lunghezza della
parola, svuota il buffer e ricomincia da capo*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#define MAX_PAROLA 100

typedef struct {
    char parola[MAX_PAROLA];  // la parola contenuta nel buffer
    bool pieno;               // true se il buffer contiene una parola
    pthread_mutex_t mutex;    // mutex
    pthread_cond_t condition; // variabile condition
} TBuffer;

/* Prototipi delle thread functions */
void *Produttore(void *param);
void *Consumatore1(void *param);
void *Consumatore2(void *param);
bool start_vocale(char*);

int main() {
    TBuffer buffer;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.condition, NULL);
    buffer.pieno=false;

    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, Produttore, &buffer);
    pthread_create(&tid[1], &attr, Consumatore1, &buffer);
    pthread_create(&tid[2], &attr, Consumatore2, &buffer);

    int i;
    for(i=0; i<3; i++)
        pthread_join(tid[i], NULL);

    return EXIT_SUCCESS;
}

void *Produttore(void *param) {
    // COMPLETARE LA FUNZIONE
    TBuffer*buff=(TBuffer*)param;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->pieno){
            pthread_cond_wait(&buff->condition,&buff->mutex);
        }
        
        printf("Produttore -> Inserire una parola da tastiera: ");
        scanf("%s",buff->parola);
        buff->pieno=true;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->condition);
    }
}

void *Consumatore1(void *param) {
    // COMPLETARE LA FUNZIONE 
    TBuffer*buff=(TBuffer*)param;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(!buff->pieno || !start_vocale(buff->parola) ){
            pthread_cond_wait(&buff->condition,&buff->mutex);
        }
        
        printf("Consumatore1 -> parola: %s\n",buff->parola);
        buff->pieno=false;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->condition);
    }
}

void *Consumatore2(void *param) {
    // COMPLETARE LA FUNZIONE 
    TBuffer*buff=(TBuffer*)param;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(!buff->pieno || start_vocale(buff->parola) ){
            pthread_cond_wait(&buff->condition,&buff->mutex);
        }
        
        printf("Consumatore2 -> len: %ld\n",strlen(buff->parola));
        buff->pieno=false;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->condition);
    }
}

bool start_vocale(char*str){
    return (str[0]=='a' ||
     str[0]=='e' ||
     str[0]=='i' ||
     str[0]=='o' ||
     str[0]=='u') ? true: false;
}


