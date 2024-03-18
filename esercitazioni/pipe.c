/*Scrivere un programma che crea una pipe anonima, dopodich\'e crea un
processo figlio.
Il processo figlio usa la pipe per inviare al processo padre un messaggio
costituito da array di 10 interi (coi valori 0,1,2,…9) e termina.
Il processo padre legge il messaggio dalla pipe (gestendo correttamente
l’eventuale frammentazione del messaggio) e lo stampa a video.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <sys/stat.h>

#define N 10

int main(){
    int fd[2];
    int i;
    int verifica=pipe(fd);
    int messaggio[N];
    
    if(verifica==-1){
        fprintf(stderr,"Non è stata creata la pipe anonima!\n");
        return 1;
    }
    
    pid_t pid=fork();    
    
    if(pid<0){
        fprintf(stderr,"errore nella fork!\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        for(i=0; i<N; i++)
            messaggio[i]=i;
            
        printf("Sono il figlio e adesso scrivo sulla pipe\n");    
        int  inviati=write(fd[1], messaggio, sizeof(messaggio));
        printf("inviati=%d\n",inviati);
        
        if(inviati<sizeof(messaggio)){
            fprintf(stderr,"Errore nel write");
            close(fd[1]);
            return 1;
        }
        
        close(fd[1]);
        
        return 0;
    }else{
        close(fd[1]);
        
        wait(NULL);
        
        int ricevuti=read(fd[0], messaggio, sizeof(messaggio));
        
        if(ricevuti==-1){
            fprintf(stderr,"Errore nella ricezione del messaggio!\n");
            close(fd[0]);
            return 1;
        }else if(ricevuti<sizeof(messaggio))
            fprintf(stderr,"messaggio parziale!");
        
        printf("Sono il padre, ricevuti=%d, e questo è il messaggio generato dal figlio:\n",ricevuti);
        for(i=0;i<N;i++)
            printf("messaggio[%d]=%d\n",i,messaggio[i]);
        
        close(fd[0]);
        
        return 0;
    }
}

/*pipe anonime
๏ Comunicazione indiretta: un processo deve creare una pipe
๏ Scambio “byte oriented”: il sistema non preserva la delimitazione dei
messaggi
๏ Invio non bloccante; ricezione bloccante*
๏ la ricezione potrebbe terminare con un messaggio
incompleto!*/
