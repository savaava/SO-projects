/*Esercizio 1
Creare un programma che riceve, tramite un parametro sulla linea di comando, un intero N. Il thread
principale crea un thread secondario a cui passa il valore di N. Il thread secondario calcola la somma
dei numeri da 1 a N (incluso), e la restituisce tramite una variabile globale somma. Il thread principale
aspetta la terminazione del thread secondario, e stampa il valore della varibile somma.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int somma;  //vale anche per il thread figlio perchè condividono lo spazio di indirizzamento

void* sum(void*n);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non sono stati passati due stringhe da riga di comando!");
        return 1;
    }

    pthread_t tid;
    printf("Il tid:%ld\n",tid);
    
    int N=atoi(argv[1]);
    
    if(pthread_create(&tid,NULL,sum,&N) !=0){
        fprintf(stderr,"errore nella creazione del thread figlio!");
        return 2;
    }
    pthread_join(tid, NULL);
    
    printf("tid:%ld\nIl thread figlio ha finito e il suo risultato è:\nsomma=%d\n",tid,somma);
    //somma è stata modificata perchè si trova nello stesso spazio di indirizzamento
    
    return 0;
}

void* sum(void*n){
    somma=0;
    int N=*((int*)n);
    for(int i=1; i<=N; i++)  //casting
        somma=somma+i;
    
    pthread_exit(NULL);  //return NULL
}
