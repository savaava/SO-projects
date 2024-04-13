/******************************************************************************

Il programma crea un processo padre e un processo figlio. Il processo figlio deve leggere da tastiera
un insieme di valori float (il numero n di valori deve essere specificato da riga comando) inserendoli in
un array e, successivamente deve ordinarli in senso crescente mediante la funzione data. Il processo
padre stampa il vettore ordinato ricevuto dal processo figlio su standard output. La comunicazione tra
processo padre e processo figlio deve avvenire tramite un’area di memoria condivisa.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

void ordina(float array[], int n);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Inserire 1 numero da riga di comando! \n");
        return 1;
    }
    int n=atoi(argv[1]);
    if(n<=0){
        fprintf(stderr,"Inserire 1 numero >0 da riga di comando! \n");
        return 1;
    }
    int i;
    float*mess;
    int segment_id=shmget(IPC_PRIVATE, n*sizeof(float), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della mem condivisa\n");
        return 1;
    }
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella fork\n");
        shmctl(segment_id,IPC_RMID,NULL);
        return 1;
    }else if(pid==0){
        mess=(float*)shmat(segment_id,NULL,0);
        if(mess==(float*)-1){
            fprintf(stderr,"Errore nel collegamento con la mem condivisa\n");
            return 1;
        }
        
        printf("Siamo nel figlio, costruiamo un array ordinato di n=%d:\n",n);
        for(i=0; i<n; i++){
            printf("Inserire array[%d]=",i);
            scanf("%f",mess+i);
        }
        ordina(mess,n);
        
        shmdt(mess);
    }else{
        mess=(float*)shmat(segment_id,NULL,0);
        if(mess==(float*)-1){
            fprintf(stderr,"Errore nel collegamento con la mem condivisa\n");
            return 1;
        }
        wait(NULL);
        
        printf("\nSiamo nel padre, stampiamo l'array ordinato di n=%d:\n",n);
        for(i=0; i<n; i++){
            printf("array[%d]=%f\n",i,mess[i]);
        }
        
        shmdt(mess);
    }
    shmctl(segment_id,IPC_RMID,NULL);
    
    return 0;
}

void ordina(float array[], int n) {
    int i, j, jmin;
    float temp;
    for(i=0; i<n-1; i++) {
        jmin=i;
        for(j=i+1; j<n; j++)
            if (array[j]<array[jmin])
                jmin=j;
        temp=array[i];
        array[i]=array[jmin];
        array[jmin]=temp;
    }
}

