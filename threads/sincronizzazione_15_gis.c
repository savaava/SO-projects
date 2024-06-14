/*
Scriverere un programma in C sulla sincronizzazione dei thread. Il programma è
composto da 1 thread produttore e 3 consumatori che lavorano in maniera coordinata.

Thread produttore:
    1 Chiede all'utente il numero di elementi
    2 Alloca lo spazio al buffer per ospitare gli n elementi
    3 Chiede all'utente di immettere gli n elementi
    4 Risveglia i thread consumatori
    5 si riparte dal passo 1.

Thread consumatori:
    1 FASE DI CALCOLO: il primo calcola la somma degli n elementi [calc_sum],
    il secondo li ordina sfruttando la funzione bubble_sort() [sort],
    il terzo calcola la media [average]
    2 FASE DI STAMPA: ciascun thread stampa il suo risultato
    3 FASE DI DEALLOCAZIONE: l'ULTIMO dei thread ad accedere alla struttura
    provvede a deallocarla.
    4 si riparte dal passo 1.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int* vet;
    int size;
    bool is_full;
    int thread_count[3];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void bubble_sort(int*,int);
void* prod(void* arg);
void* calc_sum(void* arg);
void* sort(void* arg);
void* average(void* arg);

int main() {
    /* INSERIRE QUI IL CODICE */
    TBuffer buff;
    buff.size=0;
    buff.is_full=false;
    buff.thread_count[0]=0; 
    buff.thread_count[1]=0;
    buff.thread_count[2]=0;
    pthread_mutex_init(&buff.mutex,NULL);
    pthread_cond_init(&buff.cond,NULL);
    
    pthread_t tid_prod,tid_sum,tid_sort,tid_average;
    
    if( pthread_create(&tid_prod, NULL, prod, &buff) !=0){
        printf("Errore nella creazione di un thread");
        return 2;
    }
    if( pthread_create(&tid_sum, NULL, calc_sum, &buff) !=0){
        printf("Errore nella creazione di un thread");
        return 2;
    }
    if( pthread_create(&tid_sort, NULL, sort, &buff) !=0){
        printf("Errore nella creazione di un thread");
        return 2;
    }
    if( pthread_create(&tid_average, NULL, average, &buff) !=0){
        printf("Errore nella creazione di un thread");
        return 2;
    }
    
    pthread_join(tid_prod,NULL);
    pthread_join(tid_sum,NULL);
    pthread_join(tid_sort,NULL);
    pthread_join(tid_average,NULL);
    
    return 0;
}

void* prod(void* arg) {
    /* INSERIRE QUI IL CODICE */
    TBuffer*buff=(TBuffer*)arg;
    int i;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(buff->is_full){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        do{
            printf("Inserire n>0: ");
            scanf("%d",&buff->size);
        }while(buff->size <=0);
        buff->vet=(int*)malloc( (buff->size)*sizeof(int) );
        for(i=0; i<buff->size; i++){
            printf("Inserire buff->vet[%d]:",i);
            scanf("%d",buff->vet+i);
        }
        printf("\n");
        buff->is_full=true;
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->cond);
    }
}

void* calc_sum(void* arg) {
    /* INSERIRE QUI IL CODICE */
    TBuffer*buff=(TBuffer*)arg;
    int sum,i;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
        while(!buff->is_full || buff->thread_count[0]==1){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        sum=0;
        for(i=0; i<buff->size; i++){
            sum+=buff->vet[i];
        }
        printf("La somma dei %d è: %d\n\n",buff->size,sum);
        buff->thread_count[0]=1;
        
        if(buff->thread_count[0]*buff->thread_count[1]*buff->thread_count[2]==1){
            buff->is_full=false;
            buff->thread_count[0]=0;
            buff->thread_count[1]=0;
            buff->thread_count[2]=0;
            free(buff->vet);
        }
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->cond);
    }
}
void* sort(void* arg) {
    /* INSERIRE QUI IL CODICE */
    TBuffer*buff=(TBuffer*)arg;
    int i;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
         while(!buff->is_full || buff->thread_count[1]==1){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        bubble_sort(buff->vet, buff->size);
        printf("Il vett ordinato di %d elementi:\n",buff->size);
        for(i=0; i<buff->size; i++){
            printf("buff->vet[%d]=%d\n",i,buff->vet[i]);
        }
        printf("\n");
        buff->thread_count[1]=1;
        
        if(buff->thread_count[0]*buff->thread_count[1]*buff->thread_count[2]==1){
            buff->is_full=false;
            buff->thread_count[0]=0;
            buff->thread_count[1]=0;
            buff->thread_count[2]=0;
            free(buff->vet);
        }
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->cond);
    }
}
void* average(void* arg) {
    /* INSERIRE QUI IL CODICE */
    TBuffer*buff=(TBuffer*)arg;
    int i;
    float avg;
    
    while(true){
        pthread_mutex_lock(&buff->mutex);
        
         while(!buff->is_full || buff->thread_count[2]==1){
            pthread_cond_wait(&buff->cond,&buff->mutex);
        }
        
        avg=0;
        for(i=0; i<buff->size; i++){
            avg+=buff->vet[i];
        }
        avg=(float)avg/buff->size;
        printf("La media dei %d è: %.3f\n\n",buff->size,avg);
        buff->thread_count[2]=1;
        
        if(buff->thread_count[0]*buff->thread_count[1]*buff->thread_count[2]==1){
            buff->is_full=false;
            buff->thread_count[0]=0;
            buff->thread_count[1]=0;
            buff->thread_count[2]=0;
            free(buff->vet);
        }
        
        pthread_mutex_unlock(&buff->mutex);
        pthread_cond_broadcast(&buff->cond);
    }
}

void bubble_sort(int a[], int n) {
    int i,k;
    bool modified;

    modified = true;
    for(k=0; k<n-1 && modified; k++) {
        modified = false;
        for(i=0; i<n-k-1; i++) {
            if(a[i]>a[i+1]) {
                int tmp = a[i];
                a[i]=a[i+1];
                a[i+1]=tmp;
                modified = true;
            }
        }
    }
}

