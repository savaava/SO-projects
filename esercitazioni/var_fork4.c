#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_val;

int main() {
    pid_t pid;
    
    global_val=0;
    int*local_val=malloc(sizeof(int));
    *local_val=1;    
    
    pid = fork();
    
    if(pid < 0){
        fprintf(stderr, "Fork failed\n");
        exit(1);        
        
    }else if(pid==0){    
        global_val=100;
        (*local_val)+=1;
        
        printf("\nnel figlio: \nglobal_val:%d\nlocal_val:%d\n\n",global_val,*local_val);        
        //in argv 1 ci metto -al.
        execlp("/bin/ls","ls",NULL);
        //io ho perso il processo figlio sicuramente perchè sovrascritto
        printf("i am the line after executing directory listing: ls - all\n");
        
    }else{    
        printf("prima della wait nel padre: \nglobal_val:%d \nlocal_val:%d\n",global_val,*local_val);        
        wait(NULL);
        printf("\ndopo la wait nel padre: \nglobal_val:%d \nlocal_val:%d\n",global_val,*local_val);
        //le variabili global e local restano 0 e 1 perchè il figlio è l'esatta copia dello spazio di indirizzamento del padre, copia quindi anche l'heap non solo lo stack. Quindi nel figlio quando modifichiamo global e local lo facciamo solo nello spazio stack e heap del figlio. Mentre la memoria heap e stack del padre rimane la stessa, questo spiega l'ultima prinf
        
        exit(0);
    }        
}
