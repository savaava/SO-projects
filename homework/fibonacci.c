/******************************************************************************

Il candidato completi il programma fornito, implementando il main. Il
programma ottiene un numero intero N dai parametri della linea di comando, e
crea un processo figlio che, utilizzando la funzione fornita 'genera_array', genera
un array contenente i primi N numeri di Fibonacci. Questo array deve essere
passato al processo padre utilizzando la memoria condivisa. Il processo padre
attende la terminazione del processo figlio, e stampa a video l'array ricevuto.

 Esempio: Specificando sulla linea di comando il parametro 10 l'output del
programma sarà:

 Array ricevuto: 1 1 2 3 5 8 13 21 34 55

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

void genera_array(int segment_id, int n);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Inserire 1 numero intero!\n");
        return 1;
    }
    
    int n=atoi(argv[1]);
    int segment_id=shmget(IPC_PRIVATE, n*sizeof(int), S_IRUSR|S_IWUSR);
    if(segment_id<0){
        fprintf(stderr,"Errore nella creazione della memoria condivisa\n");
        return 1;
    }
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }else if(pid==0){
        genera_array(segment_id, n);
        return 0;
    }else{
        int*mess=(int*)shmat(segment_id, NULL, 0);
        wait(NULL);
        
        printf("Array ricevuto: ");
        for(int i=0; i<n; i++){
            printf("%d ",mess[i]);
        }
        printf("\n");
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}

void genera_array(int segment_id, int n){
    int*mess=(int*)shmat(segment_id,NULL,0);
    
    mess[0]=1;
    mess[1]=1;    
    for(int i=2;i<n;i++)
        mess[i]=mess[i-1]+mess[i-2];
    
    shmdt(mess);
    return 0;
}
