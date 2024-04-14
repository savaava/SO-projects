/*Scrivi un programma in C che crea un processo figlio.
Il processo padre deve creare un segmento di memoria condivisa. 
Il processo padre genera un array di numeri casuali e lo scrive nel segmento di memoria condivisa. 
Il processo figlio legge l’array dalla memoria condivisa, 
calcola la somma dei numeri e la scrive nel segmento di memoria condivisa. Infine, 
il processo padre legge la somma dalla memoria condivisa e la stampa*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/stat.h>   
#include <sys/shm.h>  

#define n 7

typedef struct{
    int vett[n];
    int somma;
}TS;

int main(){
    TS*mess;
    
    int segment_id=shmget(IPC_PRIVATE, sizeof(TS), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della mem condivisa\n");
        return 1;
    }
    
    int i;
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella fork");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }else if(pid==0){ //figlio
        usleep(1000);    
        mess=(TS*)shmat(segment_id, NULL, 0);
        if(mess==(TS*)-1){
            fprintf(stderr,"Errore nel collegamento alla mem condivisa\n");
            return 1;
        }        
        
        printf("Sono il figlio -> calcolo la somma dei %d elementi:\n",n);
        mess->somma=0;
        for(i=0; i<n; i++)
            mess->somma +=mess->vett[i];
        
        shmdt(mess);
    }else{ //padre
        mess=(TS*)shmat(segment_id, NULL, 0);
        if(mess==(TS*)-1){
            fprintf(stderr,"Errore nel collegamento alla mem condivisa\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }
        
        printf("Sono il padre e genero l'array di dimensione %d:\n",n);
        for(i=0; i<n; i++){
            mess->vett[i]=rand()%100;
            printf("array[%d]=%d\n",i, mess->vett[i]);
        }
        
        wait(NULL);
        
        printf("La somma, calcolata dal figlio, è: %d\n", mess->somma);
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
    }
    
}











