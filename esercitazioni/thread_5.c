/*Esercizio 5
Scrivere in C un programma che legge da tastiera due numeri interi: n e k. Successivamente il processo
crea 3 thread secondari che provvedono a calcolare indipendentemente il fattoriale di n, quello di k e
quello di (n-k) mediante una funzione factorial_thread che richiama un’altra funzione per il calcolo
vero e proprio del fattoriale. Una volta terminati i thread secondari, il thread principale stampa i risultati
di ognuno e calcola e stampa il coefficiente binomiale
(n)
(k)=(n!)/(k!(n − k)!)*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 3

int factorial_thread(int);
void*thread_func(void*);

typedef struct{
    int n;
    int res;
}TS;

int main(){
    int n,k,i;
    
    do{
    printf("Inserire n e k >=0:\n");
    scanf("%d %d",&n,&k);
    }while(n<0 || k<0 || n<k);
    
    pthread_t tid[THREADS];
    
    //devo preparare tre strutture diverse per i tre thread:
    TS x[THREADS];
    x[0].n=n;
    x[1].n=k;
    x[2].n=n-k;
    
    for(i=0; i<THREADS; i++){
        if(pthread_create(tid+i, NULL, thread_func, x+i) !=0){
            fprintf(stderr,"Errore nella creazione di un thread\n");
            return 1;
        }
    }
    
    for(i=0; i<THREADS; i++)
        pthread_join(tid[i],NULL);
        
    printf("Il risultato del coefficiente binomiale tra n e k è: %d\n",(x[0].res)/((x[1].res)*(x[2].res)));
}

int factorial_thread(int n){
    return (n<=1) ? 1:n*factorial_thread(n-1);
}

void*thread_func(void*x){
    TS*dato=(TS*)x;
    dato->res=factorial_thread(dato->n);
}










