/*Esercizio 6 - Homework
Scrivere in C un programma che prenda come argomenti una lista di interi positivi e crea NTHREADS
thread, dove NTHREADS è una costante. Gli argomenti sono divisi in NTHREADS parti, ognuna delle quali
passata come input ad un thread. Ogni thread calcola la funzione fibonacci per ognuno degli interi in
input. Il main stampa a video la lista ordinata (successione di fibonacci) contentente i risultati calcolati
dai thread.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int const NTHREADS=4;

int fibonacci(int);
void*thread_func(void*);

typedef struct{
    int *vector;
    int length;
}TS;

int main(int argc, char*argv[]){
    if(argc==1){
        fprintf(stderr,"Inserire almeno 1 intero\n");
        return 1;
    }
    
    int i;
    int n=argc-1;
    TS dati[NTHREADS];
    pthread_t tid[NTHREADS];
    int risultato[n];
    
    for(i=0; i<n; i++){
        risultato[i]=atoi(argv[i+1]);
        if(risultato[i]<0){
            fprintf(stderr,"è stato inseritoscritto un numero negativo!\n");
            return 1;
        }
    }
    
    int conta=n;
    if(n<=NTHREADS){
        for(i=0; i<NTHREADS; i++){
            if(conta!=0){
                dati[i].length=1;
                dati[i].vector=risultato+i;
                conta--;
            }else
                dati[i].length=0; //n<NTHREADS thread_func non deve fare nulla
        }
    }else{ //n>NTHREADS
        int div=n/NTHREADS;
        div+=1; //per quando la divisione approssima
        for(i=0; i<NTHREADS; i++){
            if(conta>=div){
                dati[i].length=div;
                dati[i].vector=risultato+i*div;
                conta-=div;
            }else{
                dati[i].length=conta; //se div=n/NTHREADS non dovesse essere intero allora c'è un thread che lavora su meno interi
                dati[i].vector=risultato+i*div;
            }
        }
    }
    
    for(i=0; i<NTHREADS; i++){
        printf("dati[%d].length=%d\n",i,dati[i].length);
        if(pthread_create(tid+1,NULL,thread_func,dati+i) !=0){
            fprintf(stderr,"Errore nella creazione di un thread\n");
            return 1;
        }
    }
    
    for(i=0; i<NTHREADS; i++)
        pthread_join(tid[i],NULL);
    
    printf("Stampa della lista di fibonacci:\n");
    for(i=0; i<n; i++)
        printf("risultato[%d]=%d\n",i,risultato[i]);
    
}


int fibonacci(int n){
    if(n==0)
        return 0;
    if(n==1)
        return 1;
    return fibonacci(n-1)+fibonacci(n-2);
}

void*thread_func(void*x){
    TS*dati=(TS*)x;
    
    for(int i=0; i<dati->length; i++)
        dati->vector[i]=fibonacci(dati->vector[i]);
}







