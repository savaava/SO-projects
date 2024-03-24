/*๏	Scrivere un programma in linguaggio C chiamato time.c che determina la quantità di tempo necessaria ad eseguire un comando da linea di comando. Il programma deve prendere in ingresso da linea comando il comando da eseguire:
./	time <command>
๏	e deve stampare a video il tempo trascorso (in micro secondi) per l’esecuzione del comando passato come argomento.
๏	Scrivere una soluzione basata su memoria condivisa.
๏	Nota: Si usi la funzione gettimeofday() per ottenere l’ora corrente:*/

/*struct timeval current
gettimeofday(&current, NULL)
// current.tv_sec represents seconds
// current.tv_usec represents microseconds*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char*argv[]){
    if(argc==1){
        printf("L'utente non ha inserito il comando\n");
        return 1;
    }

    int fd[2];
    long int msg;
    struct timeval current;
    
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe\n");
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        char pathname[100];
        strcpy(pathname,"/bin/");
        strcat(pathname,argv[1]);
        printf("Il comando inserito è: %s\n",pathname);
        
        gettimeofday(&current, NULL);
        msg=current.tv_usec;
        printf("msg=%ld\n",msg);
        if(write(fd[1], &msg, sizeof(long int)) < sizeof(long int))
            return -1;
        printf("Sono qui\n");
        execlp(argv[1],argv[1],NULL);
        printf("Il figlio non arriverà qui");
    }else{
        close(fd[1]);
        printf("Sono il padre\n");
        if(read(fd[0], &msg, sizeof(long int)) ==-1)
            return -1;
        gettimeofday(&current, NULL);
        printf("Il tempo necessario per eseguire %s è %ld microsecondi\n",argv[1],current.tv_usec-msg);
        close(fd[0]);
    }
}
