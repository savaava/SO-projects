#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <sys/stat.h>

#define N 1

int main(){
    int fd[2];
    int i;
    int verifica=pipe(fd);
    int messaggio;
    
    if(verifica==-1){
        fprintf(stderr,"Non è stata creata la pipe anonima!\n");
        return 1;
    }
    
    pid_t pid=fork();    
    
    if(pid<0){
        fprintf(stderr,"errore nella fork!\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        printf("Inserire il messagio intero:");
        scanf("%d",&messaggio);
              
        int  inviati=write(fd[1], &messaggio, sizeof(messaggio));
        printf("inviati=%d\n",inviati);
        
        if(inviati<sizeof(messaggio)){
            fprintf(stderr,"Errore nel write");
            close(fd[1]);
            return 1;
        }
        
        close(fd[1]);
        
        return 0;
    }else{
        close(fd[1]);
        
        int ricevuti=read(fd[0], &messaggio, sizeof(messaggio));
        
        if(ricevuti==-1){
            fprintf(stderr,"Errore nella ricezione del messaggio!\n");
            close(fd[0]);
            return 1;
        }else if(ricevuti<sizeof(messaggio))
            fprintf(stderr,"messaggio parziale!");
        
        printf("Sono il padre, ricevuti=%d, e questo è il messaggio generato dal figlio:\n",ricevuti);
        printf("E il mesagio è: %d\n",messaggio);
        
        close(fd[0]);
        
        return 0;
    }
}
