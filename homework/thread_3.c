/*Esercizio 3
Scrivere in C un programma che prenda come parametro sulla linea di comando un numero intero n e
crea n threads. Ciascun thread stampa il proprio indice. L’indice del thread è così determinato: il primo thread creato ha indice 0, il secondo 1, ..., l’n-esimo ha indice n-1.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void*lettura(void*arg);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non sono stati passati due stringhe da riga di comando!");
        return 1;
    }
    
    int n=atoi(argv[1]);
    int i,arg[n];
    pthread_t tids[n];
    
    for(i=0; i<n; i++){
        arg[i]=i;
        if(pthread_create(tids+i,NULL,lettura,arg+i) !=0){
            fprintf(stderr,"errore nella creazione del thread figlio!");
            return 2;
        }
        printf("Sono qui\n");
    }
    for(i=0; i<n; i++)
        pthread_join(tids[i],NULL);
    
    return 0;
}

void*lettura(void*arg){
    int indice_curr=*((int*)arg);
    printf("L'indice del thread corrente è: %d\n",indice_curr);
    pthread_exit(NULL);
}
