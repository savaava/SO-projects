/*Scrivere in C un programma che prenda come parametro sulla linea di comando un numero intero n e
crea n threads. Ciascun thread stampa il proprio indice. L’indice del thread è così determinato: il primo
thread creato ha indice 0, il secondo 1, ..., l’n-esimo ha indice n-1.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_stampa_index(void* index);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Inserire un numero n da riga di comando\n");
        return 1;
    }
    
    int n=atoi(argv[1]);
    if(n<0){
        fprintf(stderr,"Inserire un numero n>=0 da riga di comando\n");
        return 1;
    }
    
    pthread_t tid[n];
    int tmp[n];
    
    for(int i=0; i<n; i++){
        tmp[i]=i;
        if(pthread_create(tid+i, NULL, thread_stampa_index, tmp+i)!=0){
            fprintf(stderr,"errore nella creazione del thread\n");
            return 1;
        }
    }
    
    for(int i=0; i<n; i++){
        pthread_join(tid[i],NULL);
    }
    
}

void* thread_stampa_index(void* index){
    printf("Sono il thread con indice: %d \n",*((int*)index));
    
    pthread_exit(NULL);
}
