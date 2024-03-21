#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 50

typedef struct s{
    float x;
    float y;
    float z;
}Ts;

int main(){
    int fd[2];
    int inviati,ricevuti,i;
    Ts coords;
    
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima\n");
        return 1;
    } 
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"errore nella fork!\n");
        return 1;
    }else if(pid==0){
        close(fd[1]);
        
        for(i=0;i<N;i++){        
        ricevuti=read(fd[0],&coords,sizeof(coords));
        if(ricevuti==-1){
            printf("Errore nella ricezione\n");
            return 1;
        }else if(ricevuti<sizeof(coords))
            printf("Messaggio parziale, perchè ricevuti:%d.\n",ricevuti);
            
        printf("Stampo la struttura dal figlio: x:%.2f y:%.2f z:%.2f\n",coords.x,coords.y,coords.z);
        }
        
        close(fd[0]);
        return 0;
    }else{
        close(fd[0]);
        
        for(i=0;i<N;i++){        
            coords.x=i;
            coords.y=i*2;
            coords.z=3*i/(2.0);
            printf("Stampo la struttura dal padre: x:%.2f y:%.2f z:%.2f\n",coords.x,coords.y,coords.z);
            inviati=write(fd[1],&coords,sizeof(coords));            
            if(inviati<sizeof(coords)){
                printf("Errore");
                return 1;
            }            
        }
        
        close(fd[1]);
        return 0;
    }
}
