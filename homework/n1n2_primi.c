/******************************************************************************
Il programma ottiene un numero intero N dai parametri della linea di comando, e crea un processo figlio
che, utilizzando la funzione fornita genera_risultato, calcola i due piu’ piccoli numeri primi maggiori di
N. I valori calcolati devono essere inviati al processo padre tramite l’uso di una pipe. Il processo padre
attende il risultato dal processo figlio, e stampa a video il valore ricevuto. Gestire in maniera appropriata
(segnalando un messaggio di errore) i casi in cui il processo padre ha un errore di ricezione oppure
riceve un messaggio incompleto.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

bool primo(int n);
void genera_risultato(int k, int *n1, int *n2);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Inserire un numero da riga di comando!\n");
        return 1;
    }
    int N=atoi(argv[1]);
    if(N<=0){
        fprintf(stderr,"Inserire un numero intero da riga di comando!\n");
        return 1;
    }
    int fd[2],mess[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima!\n");
        return 1;
    }
    pid_t pid=fork();
    
    if(pid<0){
        
    }else if(pid==0){
        close(fd[0]);
        
        genera_risultato(N,mess,mess+1);
        
        if(write(fd[1], mess, 2*sizeof(int)) <2*sizeof(int)){
            fprintf(stderr,"Errore nella trasmissione!\n");
            close(fd[1]);
            return 1;
        }
        
        close(fd[1]);
    }else{
        close(fd[1]);
        
        int ricevuti=read(fd[0], mess, 2*sizeof(int));
        if(ricevuti==-1){
            fprintf(stderr,"Errore nella ricezione!\n");
            close(fd[0]);
            return 1;
        }else if(ricevuti<2*sizeof(int))
            printf("Messaggio incompleto");
        
        printf("i valori n1 ed n2 sono: %d e %d\n",mess[0],mess[1]);
        
        close(fd[0]);
    }
    
}


bool primo(int n) {
    if (n%2 == 0)
        return n==2;
    int i;
    for(i=3; i*i<=n; i+=2)
        if (n%i == 0)
            return false;
    return true;
}
/*
* Calcola il piu' piccolo numero primo maggiore di k.
* Parametri di ingresso:
* k il valore k rispetto al quale calcolare i due numeri
* Parametri di uscita
* n1 il più piccolo numero primo maggiore di k
* n2 il secondo più piccolo numero primo maggiore di k
*/
void genera_risultato(int k, int *n1, int *n2) {
    int i = k+1;
    while(!primo(i))
        i++;
    *n1 = i;
    i++;
    while (!primo(i))
        i++;
    *n2 = i;
}

