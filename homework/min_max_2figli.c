/*Scrivere un programma in cui un processo genera un figlio. Il processo figlio calcola il 
valore minimo degli elementi di un vettore di interi dichiarato e inizializzato dal padre, e 
inoltre genera a sua volta un figlio (il nipote) che calcola il valore massimo del medesimo 
vettore. Prima di terminare, il processo iniziale stampa il minimo e il massimo calcolati 
rispettivamente da figlio e nipote.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define n 6

int calcola_min(int*v);
int calcola_max(int*v);

typedef struct{
    int min;
    int max;
}TS_dati;

int main(){
    if(n<=0){
        fprintf(stderr,"n<=0\n");
        return 1;
    }
    
    int segment_id=shmget(IPC_PRIVATE, sizeof(TS_dati), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore con shmget");
        return 1;
    }
    
    int vett[n];
    vett[0]=9;
    vett[1]=0;
    vett[2]=-3;
    vett[3]=55;
    vett[4]=3;
    vett[5]=8;
    
    TS_dati*mess;
    
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella fork\n");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }else if(pid==0){ //figlio
    
        pid=fork();
        if(pid<0){
            fprintf(stderr,"Errore nella fork\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }else if(pid==0){ //nipote
        
            mess=(TS_dati*)shmat(segment_id, NULL, 0);
            if(mess==(TS_dati*)-1){
                fprintf(stderr,"Errore nel collegamento all'area di mem condivisa\n");
                shmctl(segment_id, IPC_RMID, NULL);
                return 1;
            }
            
            mess->max=calcola_max(vett);
            
            shmdt(mess);            
        }else{wait(NULL);}
        
        mess=(TS_dati*)shmat(segment_id, NULL, 0);
        if(mess==(TS_dati*)-1){
            fprintf(stderr,"Errore nel collegamento all'area di mem condivisa\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }
        
        mess->min=calcola_min(vett);
        
        shmdt(mess);
    }else{ //padre
        mess=(TS_dati*)shmat(segment_id, NULL, 0);
        if(mess==(TS_dati*)-1){
            fprintf(stderr,"Errore nel collegamento all'area di mem condivisa\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }
        
        wait(NULL);
        
        printf("Sono il padre e\nIl minimo è: %d\nIl massimo è:%d\n",mess->min,mess->max);
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
    }
}

int calcola_min(int*v){
    int min=v[0];
    for(int i=1; i<n; i++){
        if(v[i]<min)
            min=v[i];
    }
    return min;
}
int calcola_max(int*v){
    int max=v[0];
    for(int i=1; i<n; i++){
        if(v[i]>max)
            max=v[i];
    }
    return max;
}



