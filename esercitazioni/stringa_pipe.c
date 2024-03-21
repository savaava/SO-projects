#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
//#include <sys/stat.h>

#define N 50

typedef struct s{
    float x;
    float y;
    float z;
}Ts;

int main(){
    int i;
    int fd[2];
    int verifica=pipe(fd);
    if(verifica==-1){
        fprintf(stderr,"Non è stata creata la pipe anonima!\n");
        return 1;
    }
    
    Ts xyz;    
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"errore nella fork!\n");
        return 1;
    }else if(pid==0){
        close(fd[1]);
        
        for(i=0;i<N;i++){        
            int ricevuti=read(fd[0],&xyz,sizeof(xyz));  
            if(ricevuti<sizeof(xyz)){
                
            }s
            printf("\nStampo la struttura dal figlio: x:%.2f y:%.2f z:%.2f\n",xyz.x,xyz.y,xyz.z);        
        }        
        close(fd[0]);
    }else{
        close(fd[0]);
        
        for(i=0;i<N;i++){        
            xyz.x=i;
            xyz.y=i*2;
            xyz.z=3*i/2;
            int inviati=write(fd[1],&xyz,sizeof(xyz));
            if(inviati<sizeof(xyz)){
                printf("Errore");
                return 1;
            }
            printf("\nStampo la struttura dal figlio: x:%.2f y:%.2f z:%.2f\n",xyz.x,xyz.y,xyz.z);        
        }
        
        close(fd[1]);
        return 0;
    }
}
