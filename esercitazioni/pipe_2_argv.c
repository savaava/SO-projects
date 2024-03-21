#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char*argv[]){
    if(argc==1){
        fprintf(stderr,"Non è stata inserita una stringa da comando\n");
        return 1;
    }
    
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima\n");
        return 1;
    }
    
    char messaggio[strlen(argv[1])+1];
    printf("strlen di argv[1] è:%ld\nsizeof di messaggio:%ld\n\n",strlen(argv[1]),sizeof(messaggio));
    
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        printf("Sono il figlio e adesso scrivo nella pipe\n");
        
        strcpy(messaggio,argv[1]);
        int inviati=write(fd[1],messaggio,sizeof(messaggio));
        if(inviati<sizeof(messaggio)){
            fprintf(stderr,"Il messaggio mandato non è completo, errore");
            return 1;
        }
        
        close(fd[1]);
        return 0;
    }else{
        close(fd[1]);
        
        int ricevuti=read(fd[0],messaggio,sizeof(messaggio));
        if(ricevuti==-1){
            fprintf(stderr,"errore nella ricezione");
            return 1;
        }else if(ricevuti<sizeof(messaggio))
            printf("Messaggio parziale, perchè ricevuti:%d.\n",ricevuti);
        printf("Sono il padre e il messaggio inviato da comando è: %s\n",messaggio);
        
        close(fd[0]);
        return 0;
    }
}
