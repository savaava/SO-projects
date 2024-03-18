/*Scrivere un programma che crei un un’area di memoria condivisa e
successivamente crei un processo figlio. Il processo figlio inserisce
nell’area condivisa un’informazione rappresentata dalla stringa "Hello
World!" che verrà poi letta dal processo padre.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include<sys/stat.h>

#define MAX_SIZE 1

int main(){
    int segment_id=shmget(IPC_PRIVATE, MAX_SIZE, S_IRUSR|S_IWUSR);
    
    if(segment_id <0){
        fprintf(stderr,"errore di allocazione della memoria condivisa!\n");
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"errore nella creazione del processo!\n");
        return 1;
    }else if(pid==0){
        printf("Sono il figlio e carico la stringa salve_mondo\n");
        char*string=(char*)shmat(segment_id, NULL, 0);
        strcpy(string,"salve_mondo\n");
        shmdt(string);
        return 0;
    }else if(pid>0){
        wait(NULL);
        char*stringa=(char*)shmat(segment_id, NULL, 0);
        printf("Sono il padre e ");
        printf("la stringa inserita da mio figlio è: %s",stringa);
        shmdt(stringa);
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}
