/* Es. consigliato: un processo crea un figlio che a sua volta crea un nipote; il padre legge due 
numeri da tastiera e li passa al figlio; il figlio eleva i due numeri al quadrato e li passa al
nipote; il nipote li somma e li stampa su output */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, char*argv[]){
    int fd1[2],fd2[2];
    if(pipe(fd1)==-1 || pipe(fd2)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima\n");
        return 1;
    }
    int n1,n2;
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        close(fd1[0]);
        close(fd2[0]);
        close(fd1[1]);
        close(fd2[1]);
        return 1;
    }else if(pid==0){
        close(fd1[1]); //il figlio vuole solo leggere dal padre
        close(fd2[0]); //ma vuole scrivere al nipote
        
        if(read(fd1[0], &n1, sizeof(n1)) <sizeof(n1)){ //ricevo il primo numero
            fprintf(stderr,"Errore nella ricezione su pipe\n");
            return 1;
        }
        if(read(fd1[0], &n2, sizeof(n2)) <sizeof(n2)){ //ricevo il secondo numero
            fprintf(stderr,"Errore nella ricezione su pipe\n");
            return 1;
        }
        
        n1=n1*n1;
        n2=n2*n2;
        
        printf("\nSono il figlio (PID=%d) e n1=%d e n2=%d\n",getpid(),n1,n2);
        
        pid=fork();        
        if(pid<0){
          fprintf(stderr,"Errore nella creazione del nipote\n");
          return 1;
        }else if(pid==0){
          close(fd2[1]); //il nipote deve solo leggere dal figlio
          
          if(read(fd2[0], &n1, sizeof(n1)) <sizeof(n1)){ //ricevo il primo numero dal figlio
             fprintf(stderr,"Errore nella ricezione su pipe\n");
             return 1;
          }
          if(read(fd2[0], &n2, sizeof(n2)) <sizeof(n2)){ //ricevo il secondo numero dal figlio
             fprintf(stderr,"Errore nella ricezione su pipe\n");
             return 1;
          }
          
          printf("\nSono il nipote (PID=%d) e la somma finale è: %d\n",getpid(),n1+n2);
          
          close(fd2[0]);
        }else{
          if(write(fd2[1], &n1, sizeof(n1)) <sizeof(n1)){ //scrivo il primo numero al nipote
            fprintf(stderr,"Errore nella trasmissione su pipe\n");
            return 1;
          }
          if(write(fd2[1], &n2, sizeof(n2)) <sizeof(n2)){ //scrivo il secondo numero al nipote
            fprintf(stderr,"Errore nella trasmissione su pipe\n");
            return 1;
          }
        }
        
        close(fd1[0]);
        close(fd2[1]);
    }else{
        close(fd1[0]);
        
        printf("Inserire due numeri n1, n2 da tastiera:\n");
        scanf("%d %d",&n1,&n2);
        printf("\nSono il padre (PID=%d) e n1=%d e n2=%d\n",getpid(),n1,n2);
        
        if(write(fd1[1], &n1, sizeof(n1)) <sizeof(n1)){ //scrivo il primo numero al figlio
            fprintf(stderr,"Errore nella trasmissione su pipe\n");
            return 1;
        }
        if(write(fd1[1], &n2, sizeof(n2)) <sizeof(n2)){ //scrivo il secondo numero al figlio
            fprintf(stderr,"Errore nella trasmissione su pipe\n");
            return 1;
        }
        
        close(fd1[1]);
    }
}



