/*Scrivere in C un programma che prenda un numero intero n come parametro sulla linea di comando, e
costruisca un array di n float con i valori 1,2,...,n Il programma crea due thread secondari, a ciascuno
dei quali passa metà dell’array (il primo thread riceve gli elementi di indice da 0 a n/2 escluso, e il
secondo gli elementi di indice da n/2 a n escluso) Ciascun thread secondario calcola, per ciascun
elemento dell’array che ha ricevuto, la radice quadrata (usando sqrt) e la memorizza nello stesso
elemento dell’array. Il thread principale aspetta la terminazione dei due thread secondari e stampa il
valore modificato dell’array.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void*thread_func(void* );

typedef struct{
    float*array;
    int n;
}TS;

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non sono stati passati due stringhe da riga di comando!\n");
        return 1;
    }
    
    int n=atoi(argv[1]);
    float array[n];
    int i;
    pthread_t tid[2];
    TS dati[2];
    
    //io separo i parametri per chiamare separatamente i threads
    dati[0].array=array;
    dati[0].n=n/2;
    dati[1].array=array+n/2;
    dati[1].n= n-dati[0].n;
    
    for(i=0; i<n; i++)
        array[i]=i+1;
    
    for(i=0; i<2; i++){
        pthread_create(tid+i, NULL, thread_func, dati+i);
    }
    
    for(i=0; i<2; i++)
        pthread_join(tid[i],NULL);
        
    printf("Stampa array:\n");
    for(i=0; i<n; i++)
        printf("array[%d]=%g\n",i,array[i]);
    
}

void*thread_func(void*tmp){
    TS*dati=(TS*)tmp;
    
    for(int i=0; i<(dati->n); i++)
        dati->array[i] = sqrt(dati->array[i]);
    
    return NULL;
}




