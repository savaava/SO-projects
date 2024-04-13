/*Esercizio 1 MidTerm: Shared Memory
Il programma crea un processo figlio che legge tre stringhe da tastiera (supponiamo che una parola
possa contenere al più 100 caratteri, incluso il terminatore), e le comunica al processo padre; il processo
padre attende la terminazione del figlio e stampa le tre stringhe ricevute. La comunicazione tra
processo padre e processo figlio deve avvenire tramite un’area di memoria condivisa.
Suggerimento: poiché l’area di memoria condivisa deve contenere più informazioni (le tre parole),
è possibile definire e usare una struttura, oppure un array a due dimensioni, oppure usare l’aritmetica
dei puntatori per accedere alle diverse parti dell’area.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 100

typedef struct{
    char str1[100];
    char str2[100];
    char str3[100];
}TS;

int main(){    
    int segment_id=shmget(IPC_PRIVATE, sizeof(TS), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della mem condivisa\n");
        return 1;
    }
    pid_t pid=fork();
    int i;
    TS*mess;
    
    if(pid<0){
        fprintf(stderr,"Errore nella fork\n");
        shmctl(segment_id,IPC_RMID,NULL);
        return 1;
    }else if(pid==0){
        mess=(TS*)shmat(segment_id,NULL,0);
        if(mess==(TS*)-1){
            fprintf(stderr,"Errore nel collegamento\n");
            return 1;
        }
        
        printf("Sono il figlio, si devono scrivere 3 stringhe:\n");
        printf("Inserire la stringa 1: ");
        scanf("%s",mess->str1);
        printf("Inserire la stringa 2: ");
        scanf("%s",mess->str2);
        printf("Inserire la stringa 3: ");
        scanf("%s",mess->str3);
        
        shmdt(mess);
    }else{
        mess=(TS*)shmat(segment_id,NULL,0);
        if(mess==(TS*)-1){
            fprintf(stderr,"Errore nel collegamento\n");
            return 1;
        }
        wait(NULL);
        printf("\nSono il padre, stampo le tre stringhe:\n");
        printf("stringa 1: %s\nstringa 2: %s\nstringa 3: %s\n",mess->str1,mess->str2,mess->str3);
        
        shmdt(mess);
        shmctl(segment_id,IPC_RMID,NULL);
    }
}
