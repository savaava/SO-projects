#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include<sys/stat.h>

#define MAX_SIZE 200

int main(){
    int segment_id=shmget(IPC_PRIVATE, MAX_SIZE, S_IRUSR|S_IWUSR);
    
    if(segment_id <0){
        fprintf(stderr,"errore di allocazione della memoria condivisa!\n");
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"errore nella creazione del processo!\n");
        shmctl(segment_id, IPC_RMID, NULL); 
        return 1;
    }else if(pid==0){
        printf("Sono il figlio,  ");
        char*string=(char*)shmat(segment_id, NULL, 0);
        printf("Inserire una parola di max %d caratteri: ",MAX_SIZE-1);
        scanf("%s",string);
        shmdt(string);
        return 0;
    }else if(pid>0){
        wait(NULL);
        char*stringa=(char*)shmat(segment_id, NULL, 0);
        printf("Sono il padre e ");
        printf("la stringa inserita da mio figlio è: %s\n",stringa);
        shmdt(stringa);
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}
