/*Esercizio 2
Modificare il programma dell’Esercizio precedente in modo da non dover usare una variabile globale
somma. In questo caso il thread deve avere accesso a due variabili intere (una in cui riceve il numero N,
e l’altra in cui mette la somma), ma la funzione del thread può avere un solo parametro. Una possibile
soluzione è usare una struttura con due campi, e passare alla funzione del thread l’indirizzo della
struttura.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* sum(void* arg);

typedef struct{
    int n;
    int somma;
}TS;

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non sono stati passati due stringhe da riga di comando!");
        return 1;
    }
    
    pthread_t tid;
    TS dati;
    dati.n=atoi(argv[1]);
    
    if(pthread_create(&tid,NULL,sum,&dati) !=0){
        fprintf(stderr,"errore nella creazione del thread figlio!");
        return 2;
    }
    
    pthread_join(tid,NULL);
    
    printf("Il thread figlio ha finito e il suo risultato è:\nsomma=%d\n",dati.somma);
    
    return 0;
}

void* sum(void* arg){
    TS*dati=(TS*)arg;
    int N=dati->n;
    dati->somma=0;
    
    for(int i=1; i<=N; i++)
        dati->somma+=i;
        
    pthread_exit(NULL);
}
