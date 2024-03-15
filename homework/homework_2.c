#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid1,pid2;
    
    //creo due figli fratelli
    pid1=fork();
    pid2=fork();//il primo figlio eseguirà questa fork e quindi genererà a sua volt un figlio.
    
    if(pid1<0 || pid2<0){
        fprintf(stderr,"fork failed!");
        exit(0);
    }else if(pid1>0 && pid2>0){
        printf("Sono il padre e\n");
        printf("PID del padre:%d\npid1:%d\npid2:%d\n",getpid(),pid1,pid2);
        wait(NULL);
    }else if(pid1==0 && pid2>0){
        printf("\nSono il figlio1 perchè pid1:%d e PID:%d\n",pid1,getpid());
        printf("Mio padre è con PID:%d\n",getppid());
        wait(NULL);
    }else if(pid1>0 && pid2==0){
        printf("\nSono il figlio2 perchè pid2:%d e PID:%d\n",pid2,getpid());
        printf("Mio padre è con PID:%d\n",getppid());
    }else{
        //Questo è il figlio del figlio1
        printf("\nSono il figlio3 perchè pid1:%d e pid2:%d e PID:%d\n",pid1,pid2,getpid());
        printf("Mio padre è con PID:%d\n",getppid());
    }
    
    return 0;-+
}
