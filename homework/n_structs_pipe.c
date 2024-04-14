/*
Scrivere un programma che crea una pipe anonima, dopodiché crea un
processo figlio
Il processo padre crea n struct che contengono coordinate reali (x,y,z) e
le invia una alla volta al processo figlio tramite la pipe. Le coordinate
devono essere settate come: x = i; y = i*2; z = 3*i/2 con i=0,…,n-1.
Il processo figlio legge dalla pipe le stuct inviate dal padre e le stampa.
Nella lettura, gestire il fatto che il messaggio sia incompleto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    float x;
    float y;
    float z;
}Tcoords;

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Inserire solo 1 numero!\n");
        return 1;
    }
    
    int n=atoi(argv[1]);
    if(n<=0){
        fprintf(stderr,"Inserire un numero n>0\n");
        return 1;
    }
    
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima\n");
        return 1;
    }
    
    Tcoords mess;
    int i;
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella fork");
        return 1;
    }else if(pid==0){ //figlio
        close(fd[1]); //il figlio deve leggere le n strutture dalla pipe
        
        for(i=0; i<n; i++){
            if(read(fd[0], &mess, sizeof(Tcoords))<sizeof(Tcoords)){
                fprintf(stderr,"Errore nella ricezione\n");
                return 1;
            }
            
            printf("Sono il figlio -> ricevo    la struttura %d: (x=%.2f, y=%.2f, z=%.2f)\n",i+1,mess.x,mess.y,mess.z);
        }
        
        close(fd[0]);
    }else{ //padre
        close(fd[0]); //il padre deve scrivere le n strutture sulla pipe
        
        for(i=0; i<n; i++){
            mess.x=i;
            mess.y=2*i;
            mess.z=(3.0)*i/2;
            
            printf("Sono il padre  -> trasmetto la struttura %d: (x=%.2f, y=%.2f, z=%.2f)\n",i+1,mess.x,mess.y,mess.z);
            if(write(fd[1], &mess, sizeof(Tcoords)) <sizeof(Tcoords)){
                fprintf(stderr,"Errore nella trasmissione\n");
                return 1;
            }
            
            usleep(1000);
        }
        
        close(fd[1]);
    }
}






