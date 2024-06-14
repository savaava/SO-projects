/*-------------------------------------------------------------------
 * Completare il programma fornito, completando il main e
 * implementando la funzione worker_thread.
 * Il main deve creare 100 thread figli, a cui deve passare
 * come parametri un intero da 1 a 100 (diverso per ogni thread figlio)
 * e un buffer che può contenere un singolo numero intero.
 * Una volta che i thread figli sono stati creati, il main deve
 * eseguire il seguente algoritmo:
 *         1. Ripeti per 100 volte i passi da 2 a 4.
 *           2. Aspetta che il buffer sia pieno.
 *           3. Preleva il valore contenuto nel buffer e stampalo a video
 *           4. Rendi il buffer vuoto.
 *
 * I thread figli, che eseguono la funzione worker_thread, devono svolgere
 * il seguente algoritmo:
 *           1. Aspetta 3 secondi.
 *           2. Aspetta che il buffer sia vuoto.
 *           3. Inserisci il numero ricevuto come parametro nel buffer.
 *           4. Rendi il buffer pieno.
 *
 * È necessario gestire in maniera appropriata la sincronizzazione dei
 * thread (incluso il main) nell'accesso al buffer.
 *
 * ESEMPIO
 * Lanciando il programma, l'ouput deve essere simile al seguente:
 *    Ho ricevuto il numero: 1
 *    Ho ricevuto il numero: 94
 *    Ho ricevuto il numero: 59
 *    Ho ricevuto il numero: 19
 *    Ho ricevuto il numero: 22
 *            . . . ( altri valori omessi per brevità ) . . .
 *    Ho ricevuto il numero: 39
 *    Ho ricevuto il numero: 40
 *    Ho ricevuto il numero: 51
 *    Ho ricevuto il numero: 56
 *
 *
 * Per testare il programma cliccare sul seguente link:
 *
 * https://onlinegdb.com/SHjR5haaA
 *
 * Una volta collegati, premere il pulsante FORK THIS di fianco al
 * pulsante RUN in alto a sinistra. Inserire nella finestra principale
 * il codice sorgente completo, inserire i parametri da passare sulla
 * linea comando nella casella CommandLine Arguments e cliccare sul
 * pulsante RUN per visualizzare il risultato dell'esecuzione.
 --------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_WORKERS 50

/* Tipo da usare per il buffer */
typedef struct {
    int valore;   /* Valore contenuto nel buffer */
    bool pieno;   /* true se il buffer contiene un valore */
    pthread_mutex_t mutex; /* Mutex per l'accesso al buffer */
    pthread_cond_t  cond;  /* Condition per l'accesso al buffer */
} TBuffer;

/* Tipo da usare per passare i parametri ai thread figli */
typedef struct {
    int numero;   /* Numero assegnato al thread */
    TBuffer *buffer; /* Puntatore al buffer */
} TWorkerParam;

/* Prototipo della funzione dei thread figli */
void *worker_thread(void*);

int main(){
    // INSERIRE QUI IL CODICE
    int i;
    
    TBuffer buff;
    buff.pieno=false;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.cond,NULL);
    
    TWorkerParam param[NUM_WORKERS];
    for(i=0; i<NUM_WORKERS; i++){
        param[i].numero=i+1;
        param[i].buffer=&buff;
    }
    
    pthread_t tid[NUM_WORKERS];
    
    for(i=0; i<NUM_WORKERS; i++){
        if( pthread_create(tid+i, NULL, worker_thread, param+i) !=0){
            fprintf(stderr,"Errore nella creazione di un thread");
            return 1;
        }
    }
    
    while(1){
        pthread_mutex_lock(&buff.mutex);
        
        while(buff.pieno==false){
            pthread_cond_wait(&buff.cond,&buff.mutex);
        }
        
        printf("Ho ricevuto il numero: %d\n",buff.valore);
        buff.pieno=false;
        
        pthread_cond_broadcast(&buff.cond);  
        pthread_mutex_unlock(&buff.mutex);
    }
    
    return 0;
}

void *worker_thread(void *param){
    /* INSERIRE IL CODICE */
    TWorkerParam*tmp=(TWorkerParam*)param;
    int x=tmp->numero; //è il numero di ogni thread che deve inserire nel buffer
    TBuffer*buff=tmp->buffer;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->pieno==true){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        buff->valore=x;
        //printf("Ho scritto il numero: %d\n",buff->valore);
        buff->pieno=true;
        
        pthread_cond_broadcast(&buff->cond);
        pthread_mutex_unlock(&buff->mutex);
        sleep(3);
    }

    return NULL;
}









