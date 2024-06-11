#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    float average;
    int sample;
    int count;
    bool is_full;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void* prod(void*);
void* calc(void*);

int main(){
    srand(time(NULL));
    
    TBuffer buffer;
    
    buffer.average=0;
    buffer.is_full=false;
    buffer.count=0;
    pthread_mutex_init(&buffer.mutex,NULL);
    pthread_cond_init(&buffer.cond,NULL);
    
    pthread_t tid_p,tid_c;
    
    if(pthread_create(&tid_p,NULL,prod,&buffer)!=0){
        fprintf(stderr,"Errore nella creazione del thread");
        return 1;
    }
    if(pthread_create(&tid_c,NULL,calc,&buffer)!=0){
        fprintf(stderr,"Errore nella creazione del thread");
        return 1;
    }
    
    pthread_join(tid_p,NULL);
    pthread_join(tid_c,NULL);

    return 0;
}


void* prod(void*arg){
    TBuffer*buff=(TBuffer*)arg;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->is_full==true){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        buff->sample=rand() % 100;
        printf("Il produttore genera il nuovo sample: %d\n",buff->sample);
        buff->is_full=true;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_signal(&buff->cond);
        sleep(4);
    }
}
void* calc(void*arg){
    TBuffer*buff=(TBuffer*)arg;
    
    while(1){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->is_full==false){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        buff->average= (float)(buff->average*buff->count + buff->sample) / (buff->count+1);
        printf("Il calcolatore ha generato la nuova media: %.3f\n",buff->average);
        buff->count++;
        buff->is_full=false;
        
        pthread_cond_signal(&buff->cond);
        pthread_mutex_unlock(&buff->mutex);
    }
}








