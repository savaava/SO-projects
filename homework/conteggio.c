/*******************************************************************************

Il candidato completi il programma fornito, implementando il main. Il programma crea un processo figlio.
Il processo figlio legge da tastiera un numero intero N ≥ 0, e trasmette al processo padre mediante una
pipe i valori N,N −1,N −2,N −3, . . . , 0 (incluso). Il processo padre legge dalla pipe i valori trasmessi
dal processo figlio e li stampa, finché non riceve il valore 0; dopodiché il processo padre attende la
terminazione del processo figlio e termina.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr, "Inserire da riga di comando 1 solo numero intero\n");
        return EXIT_FAILURE;
    }
    
    int x=atoi(argv[1]);
    if(x<0){
        fprintf(stderr, "Inserire da riga di comando un numero >= 0\n");
        return EXIT_FAILURE;
    }    
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe\n");
        return 1;
    }    
    pid_t pid=fork();
    int mess;
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del processo figlio\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        /*for(int i=0; i<=x; i++){
            v[i]=x-i;
            printf("(figlio) i=%d -> %d\n",i,v[i]);
        }
        write(fd[1], v, sizeof(v));*/
        
        for(int i=0; i<=x; i++){
            mess=x-i;
            printf("(figlio) i=%d -> %d\n",i,mess);
            write(fd[1], &mess, sizeof(int));
        }
        
        close(fd[1]);
    }else{  
        close(fd[1]);
        
        int ricevuti;
        
        /*read(fd[0], v, sizeof(v));
        for(int i=0; i<=x; i++){
            printf("(padre) i=%d -> %d\n",i,v[i]);
        }*/
        
        printf("padre: ");
        for(int i=0; i<=x; i++){
            ricevuti=read(fd[0], &mess, sizeof(int));
            printf("%d ",mess);
        }
        printf("\n");
        
        close(fd[0]);
    }
}
