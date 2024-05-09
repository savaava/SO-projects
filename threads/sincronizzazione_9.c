/*Esercizio 9 (Sincronizzazione)
Scrivere un programma in C che implementi il problema produttore/consumatore con i threads sincronizzati
mediante mutex e variabili condition. Produttori e consumatori hanno un proprio numero
identificativo, assegnato in fase di creazione Le informazioni scambiate tra produttori e consumatori
sono numeri interi. Il thread principale crea 1 produttore e 2 consumatori. Il produttore produce numeri
progressivi (1, 2, …), aspettando un tempo prestabilito tra un numero e l’altro Ciascun consumatore
preleva i numeri dal buffer, li stampa a video e aspetta un tempo prestabilito tra un numero e l’altro.
Il massimo numero di informazioni da mantenere nel buffer è 4. Il thread principale termina dopo 60
secondi.*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PROD_THREADS 1
#define CONS_THREADS 2

#define BUF_SIZE 4

#define MAIN_SLEEP 60

typedef struct {
    int elem[BUF_SIZE]; /* Elementi del buffer */
    int primo;   /* Indice del primo elemento */
    int ultimo;  /* Indice successivo all'ultimo elemento */
    int cont;    /* Numero di elementi nel buffer */
    pthread_mutex_t mutex;
    pthread_cond_t non_pieno;
    pthread_cond_t non_vuoto;
} TBuffer;


typedef struct {
    TBuffer *buffer; /* Puntatore al buffer condiviso */
    int id; /* Identificativo del produttore/consumatore */
    int ritardo; /* Durata del ritardo in secondi */
} TParam;

void *produttore(void *arg);
void *consumatore(void *arg);

int main() {
    //preparo il buffer condiviso
    TBuffer buffer;
    buffer.primo=0;
    buffer.ultimo=0;
    buffer.cont=0;
    pthread_mutex_init(&buffer.mutex,NULL);
    pthread_cond_init(&buffer.non_vuoto,NULL);
    pthread_cond_init(&buffer.non_pieno,NULL);
    
    pthread_t tid_prod[PROD_THREADS];
    pthread_t tid_cons[CONS_THREADS];
    TParam dati_prod[PROD_THREADS];
    TParam dati_cons[CONS_THREADS];
    
    int i;
    
    for(i=0; i<PROD_THREADS; i++){
        dati_prod[i].buffer=&buffer;
        dati_prod[i].id=i+1;
        dati_prod[i].ritardo=3;
    }
    for(i=0; i<CONS_THREADS; i++){
        dati_cons[i].buffer=&buffer;
        dati_cons[i].id=i+1;
        dati_cons[i].ritardo=10;
    }
    
    for(i=0; i<PROD_THREADS; i++){
        if(pthread_create(tid_prod+i, NULL, produttore, dati_prod+i)!=0){
            
        }
    }
    for(i=0; i<CONS_THREADS; i++){
        if(pthread_create(tid_cons+i, NULL, consumatore, dati_cons+i)!=0){
            
        }
    }
    
    sleep(MAIN_SLEEP); //appena terminano i 60 sec i threads di interrompono
    return 0;
}


void *produttore(void *arg){
    TParam*dati_prod=(TParam*)arg;
    TBuffer*buffer=dati_prod->buffer;
    int n=0;
    
    while(1){
        n++;
        pthread_mutex_lock(&buffer->mutex);
        
        while(buffer->cont == BUF_SIZE)
            pthread_cond_wait(&buffer->non_pieno, &buffer->mutex); 
            /*se non pieno è una condizione verificata, allora va avanti perchè
            non entra nel while*/
            /*se non pieno non è una cond verificata, allora entra nel while ed 
            esegue implicitamente una signal sul mutex e il processo si sospende aspettando la
            variabile condition non_pieno che sia verificata
            che si sbloccherà quando qualcuno farà la signal su non_pieno*/
        
        buffer->elem[buffer->ultimo] = n;
        buffer->cont++;
        buffer->ultimo = (buffer->ultimo+1) % BUF_SIZE;
        
        pthread_cond_signal(&buffer->non_vuoto);
        /*sblocca un processo che è in attesa di non_vuoto, ossia un consumatore
        Quando il consumatore viene risvegliato esegue implicitamente una wait sul mutex*/
        
        pthread_mutex_unlock(&buffer->mutex);
        
        printf("sono il PRODUTTORE con id=%d e ho prodotto n=%d\n",dati_prod->id,n);
        sleep(dati_prod->ritardo);
    }
}
void *consumatore(void *arg){
    TParam*dati_cons=(TParam*)arg;
    TBuffer*buffer=dati_cons->buffer;
    int n;
    
    while(1){
        pthread_mutex_lock(&buffer->mutex);
        
        while(buffer->cont == 0)
            pthread_cond_wait(&buffer->non_vuoto, &buffer->mutex);
            /*se non vuoto è verificata, allora va avanti perchè non entra nel while*/
            /*se non vuoto non è verificata, allora entra nel while ed esegue 
            una signal sul mutex e il processo aspetta sulla variabile condition non_vuoto
            che si sbloccherà quando qualcuno farà la signal su non_vuoto*/
        
        n=buffer->elem[buffer->primo];
        buffer->cont--;
        buffer->primo=(buffer->primo+1) % BUF_SIZE;
        
        pthread_cond_signal(&buffer->non_pieno);
        /*risveglia un processo che è in attesa di non_pieno, ossia un produttore
        quando il produttore viene risvegliato effettua implicitamente una wait sul mutex*/
        
        pthread_mutex_unlock(&buffer->mutex);
        
        printf("sono il consumatore con id=%d e ho consumato il n=%d\n",dati_cons->id,n);
        sleep(dati_cons->ritardo);
    }
}
















