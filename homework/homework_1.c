#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"fork failed with pid:%d <0\n",pid);
        exit(0);
    }else if(pid==0){
        printf("\nSono il figlio1 (pid:%d e PID:%d)\n",pid,getpid());
        printf("Infatti mio padre ha pid:%d\n",getppid());
        pid=fork();
        
        if(pid<0){
            fprintf(stderr,"fork failed with pid:%d <0\n",pid);
            exit(1);
        }else if(pid==0){
            printf("Sono il figlio2 (pid:%d e PID:%d)\n",pid,getpid());
            printf("Infatti mio padre è il figlio1 con pid:%d\n",getppid());
        }else{
            printf("Sono il figlio1, ossia il nuovo padre, e mio figlio2 ha pid:%d\n",pid);
            wait(NULL);        
        } 
        
    }else{
        printf("Sono il padre con PID:%d e mio figlio ha pid:%d\n",getpid(),pid);
        wait(NULL);     
        printf("\nSono il padre iniziale.\n");
    }   
    
    //sono stati creati 3 processi: il nonno, ossia il main, il figlio1 e poi il figlio2.
    
    return 0;
}
