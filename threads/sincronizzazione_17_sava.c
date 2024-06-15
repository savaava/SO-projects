/* sava
Il programma prevede di ricevere da linea di comando un numero pari N, 1<N<51.
Si creino N threads, di cui la prima metà sono produttori e la seconda i consumatori.
Ogni produttore  ha un proprio id che va da 1 a N/2
Ogni consumatore ha un proprio id che va da 1 a N/2 

Ogni produttore chiede da tastiera una stringa, attraverso un prompt che deve 
specificare il proprio id: (N=10) output ->
PRODUTTORE (id=4) -> richiedo stringa: ciaoo
Ora può riempire il buffer condiviso 

Ogni consumatore legge dal buffer, stampando:
CONSUMATORE (id=5) -> prelevo stringa: ciaoo, dal produttore (id=4)

Si adotti un'implementazione della sincronizzazione tramite l'utilizzo di due 
condvar (vuoto,pieno) da parte dei produttori e consumatori, nonostante ne basti una.

Il thread principale è in attesa infinita, per cui il programma non deve terminare
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    int x;
    char str[100];
}Telementi;

typedef struct{
    Telementi e;
    bool completo;
    pthread_mutex_t mutex;
    pthread_cond_t vuoto;
    pthread_cond_t pieno;
}TBuffer;

typedef struct{
    int id;
    TBuffer* buffer;
}Tparam;

void* produttore(void*);
void* consumatore(void*);

int main(int argc, char* argv[]){
    //INSERIRE IL CODICE
    if (argc!=2){
        printf("Inserire solo N pari, 1<N<51\n");
        return 1;
    }
    
    int N=atoi(argv[1]);
    if(N<2 || N>50 || N%2==1){
        printf("Inserire solo N pari, 1<N<51\n");
        return 1;
    }
    
    TBuffer buff;
    buff.completo=false;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.vuoto,NULL);
    pthread_cond_init(&buff.pieno,NULL);
    
    Tparam prod[N/2];
    Tparam cons[N/2];
    
    int i;
    for(i=0; i<N/2; i++){
        prod[i].id=i+1;
        prod[i].buffer=&buff;
        cons[i].id=i+1;
        cons[i].buffer=&buff;
    }
    
    pthread_t tid_prod[N/2];
    pthread_t tid_cons[N/2];
    
    for(i=0; i<N/2; i++){
        if(pthread_create(tid_prod+i, NULL, produttore, prod+i)!=0){
            printf("Err nella creazione di un thread\n");
            return 2;
        }
        if(pthread_create(tid_cons+i, NULL, consumatore, cons+i)!=0){
            printf("Err nella creazione di un thread\n");
            return 2;
        }
    }
    
    for(i=0; i<N/2; i++){
        pthread_join(tid_prod[i],NULL);
        pthread_join(tid_cons[i],NULL);
    }
    
}

void* produttore(void*arg){
    //INSERIRE IL CODICE 
    Tparam*tmp=(Tparam*)arg;
    int id=tmp->id;
    TBuffer*buff=tmp->buffer;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->completo){
            pthread_cond_wait(&buff->vuoto,&buff->mutex);
        }
        
        printf("PRODUTTORE (id=%d) -> richiedo stringa: ",id);
        scanf("%s",buff->e.str);
        buff->e.x=id;
        buff->completo=true;
        
        pthread_cond_signal(&buff->pieno);
        pthread_mutex_unlock(&buff->mutex);
    }
}

void* consumatore(void*arg){
    //INSERIRE IL CODICE 
    Tparam*tmp=(Tparam*)arg;
    int id=tmp->id;
    TBuffer*buff=tmp->buffer;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(!buff->completo){
            pthread_cond_wait(&buff->pieno,&buff->mutex);
        }
        
        printf("CONSUMATORE (id=%d) -> prelevo stringa: %s, dal produttore (id=%d)\n\n",id,buff->e.str,buff->e.x);
        buff->completo=false;
        
        pthread_cond_signal(&buff->vuoto);
        pthread_mutex_unlock(&buff->mutex);
    }
}











