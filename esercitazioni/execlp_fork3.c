#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_val;

int main() {
    pid_t pid;    
    pid = fork();
    
    if(pid < 0){
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }else if(pid==0){
        printf("\nSono il figlio (pid:%d e PID:%d) e adesso verrò sovrascritto da quest'altro programma che eseguirà una directory listing: ls -al\n\n",pid,getpid());
        execlp("/bin/ls","ls","-al",NULL);
        //io ho perso il processo figlio sicuramente perchè sovrascritto da quest'altro programma eseguito con execlp
        //per questo non verrà mai eseguito questo printf sotto.
        printf("i am the line after executing directory listing: ls - all\n");        
    }else{      
        printf("Il figlio ha pid:%d\n",pid);
        wait(NULL);
        printf("\nHo terminato il figlio (la execlp) e ora sono tornato dal padre\n");
        //ho sovrascritto il figlio, ma il padre continuo a eseguirlo perchè non l'ho eliminato
        exit(0);
    }        
}
