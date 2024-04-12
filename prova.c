#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SIZE 100

void child_process(int s_id, int flag);

int main(){
    
    int segment_id=shmget(IPC_PRIVATE, SIZE*sizeof(float), S_IRUSR|S_IWUSR); //per allocare una memoria condivisa 
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocaziopne della mem condivisa\n");
        return 1;
    }
    float*v; //per attaccarlo alla memoria condivisa sia nell'istanza di codice del padre sia del figlio, i quali hanno heap e stack separati perchè hanno spazi di indirizzamento uguali ma separati
    pid_t pid=fork(); //per creare un processo figlio con un certo pid
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del processo figlio\n");
        shmctl(segment_id, IPC_RMID, NULL); //per deallocare la memoria condivisa, perchè non lo fa il s.o.
        return 1;
    }else if(pid==0){
        child_process(segment_id, 1);
        
        pid=fork();
        if(pid<0){
                fprintf(stderr,"Errore nella creazione del processo figlio\n");
                shmctl(segment_id, IPC_RMID, NULL); //per deallocare la memoria condivisa, perchè non lo fa il s.o.
                return 1;
        }else if(pid==0){
            child_process(segment_id, 2);
        }else{
            wait(NULL);
        }
        
    }else{
        wait(NULL); 
        v=(float*)shmat(segment_id, NULL, 0);
        if(v==(float*)-1){
            fprintf(stderr,"Errore nell'attach della memoria\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }
        
        float somma=0;
        for(int i=0; i<SIZE; i++)
            somma=somma+v[i];
        printf("La somma è: %f\n",somma);
        
        shmdt(v);
        shmctl(segment_id, IPC_RMID, NULL);
        
        printf("il mio pid: %d\nil pid di mio figlio: %d\nil pid del bash: %d\n",getpid(),pid,getppid());
    }
    
}

void child_process(int s_id, int flag){
    printf("il mio pid: %d\nil pid di mio padre padre: %d\n",getpid(),getppid());
    
    float*v=(float*)shmat(s_id, NULL, 0);
    if(v==(float*)-1){
            fprintf(stderr,"Errore nell'attach della memoria\n");
            shmctl(s_id, IPC_RMID, NULL);
            return;
        }
    
    int i;
    if(flag==1){
        printf("Sono il figlio numero %d e scrivo sulla memoria la parte %d dell'array\n",flag,flag);
        for(i=0; i<50; i++)
            v[i]=(1.0)/(i+1);
    }else{
        printf("Sono il figlio numero %d e scrivo sulla memoria la parte %d dell'array\n",flag,flag);
        for(i=50; i<SIZE; i++)
            v[i]=(1.0)/(i+1);
    }
        
    shmdt(v);
}
