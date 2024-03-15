#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf("Il numero di stringhe da stampare è %d\n",argc-1);
    
    pid_t pid;
    
    for(int i=1; i<argc; i++){
        pid=fork();
        
        if(pid<0){
            fprintf(stderr,"fork failed");
            exit(1);
        }else if(pid==0){
            sleep(rand()%10);
            
            printf("PID: %d, Messaggio: %s\n",getpid(),argv[i]);
            printf("Mio padre ha PID:%d\n",getppid());
            exit(0);
        }else{
            printf("Sono il padre e pid:%d\n",pid);
            wait(NULL);
        }        
    }
    return 0;
}
