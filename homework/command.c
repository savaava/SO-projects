#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char*argv[]){
    if(argc-1 <=0){
        fprintf(stderr,"non è stato inserito il comando da eseguire!\n");
        exit(0);
    }else if(argc-1 >=2){
        fprintf(stderr,"è stato inserito più di 1 comando da eseguire!\n");
        exit(0);
    }
    
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"forked failed!");
        exit(1);
    }else if(pid==0){
        char pathname[100];
        strcpy(pathname,"/bin/");
        strcat(pathname,argv[1]);
        printf("Il comando inserito è: %s\n",pathname);
            
        execlp(pathname,argv[1],NULL);
        printf("\nComando non eseguito!\n");        
    }else{
        wait(NULL);
        printf("\nComando eseguito!\n");
    }
    
    return 0;
}
