#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include<sys/stat.h>

#define n 100

void child_process(int seg_id);

int main(){
    int dim= n*sizeof(float);
    int segment_id= shmget(IPC_PRIVATE, dim, S_IRUSR|S_IWUSR);
    
    if(segment_id<0){
        fprintf(stderr,"errore allocazione");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"errore nella creazione del processo!\n");
        shmctl(segment_id, IPC_RMID, NULL); 
        return 1;
    }else if(pid==0){
        child_process(segment_id);
        return 0;
    }else{
        wait(NULL);
        float*vett=(float*)shmat(segment_id, NULL, 0);
        float somma=0;
        for(int i=0; i<n; i++)
            somma=somma+vett[i];
        
        printf("La somma è: %f\n",somma);
            
        shmdt(vett);
        shmctl(segment_id, IPC_RMID, NULL);
        
        return 0;
    }    
}

void child_process(int seg_id){
    float*v=(float*)shmat(seg_id,NULL,0);
    for(int i=0; i<n; i++)
        v[i]=(1.0)/(i+1);
    shmdt(v);    
    return;
}