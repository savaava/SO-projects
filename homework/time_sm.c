#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/shm.h>
#include<sys/stat.h>

int main(int argc, char*argv[]){
    if(argc!=2){
        printf("L'utente non ha inserito un singolo comando\n");
        return 1;
    }
    
    struct timeval current;
    int segment_id=shmget(IPC_PRIVATE,sizeof(long int),S_IRUSR|S_IWUSR);
    if(segment_id<0){
        fprintf(stderr,"Errore allocazione memoria");
        return 1;
    }
    
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        return 1;
    }else if(pid==0){
        long int*tmp=(long int*)shmat(segment_id,NULL,0);
        
        char pathname[100];
        strcpy(pathname,"/bin/");
        strcat(pathname,argv[1]);
        printf("Il comando inserito è: %s\n",pathname);
        
        gettimeofday(&current, NULL);
        *tmp=current.tv_usec;
        shmdt(tmp);
        execlp(argv[1],argv[1],NULL);
        printf("Il figlio non arriverà qui se il comando è scritto correttamente\n");
    }else{
        wait(NULL);
        gettimeofday(&current, NULL);
        long int*msg=(long int*)shmat(segment_id,NULL,0);        
        printf("Il tempo necessario per eseguire /bin/%s è %ld microsecondi\n",argv[1],current.tv_usec-(*msg));
        shmdt(msg);
        shmctl(segment_id,IPC_RMID,NULL);
        return 0;
    }
}
