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

typedef struct{
    float*vett;
    int dim;
}TS;

void*thread_func(void*vett);

int main(int argc, char*argv[]){
    if(argc!=2){
        printf("Inserire un numero da riga di comando!\n");
        return 1;
    }
    int n=atoi(argv[1]);
    if(n<0){
        printf("Inserire un numero >=0 da riga di comando!\n");
        return 1;
    }
    int i;
    float*v=(float*)malloc(n*sizeof(float));
    TS dati[2];    
    dati[0].vett=v;
    dati[1].vett=v+n/2;
    dati[0].dim=n/2;
    dati[1].dim=n-n/2;
    pthread_t tid[2];
    
    for(i=0;i<n;i++)
        v[i]=i+1;
    
    for(i=0;i<2;i++){
            pthread_create(tid+i, NULL, thread_func, &dati[i]);
    }
    
    for(i=0;i<2;i++)
        pthread_join(tid[i], NULL);
    
    for(i=0;i<n;i++)
        printf("vett[%d]=%f\n",i,v[i]);
        
    free(v);
}

void*thread_func(void*vett){
    TS*dati=(TS*)vett;
    
    for(int i=0; i<dati->dim; i++){
        printf("eccomi vett[%d]=%f\n",i,dati->vett[i]);
        dati->vett[i]=sqrt(dati->vett[i]);
    }
    
    pthread_exit(NULL);
}






