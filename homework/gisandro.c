/*il programma riceve da riga di comando 1 o più comandi (ls, tree, la, pwd ...), tra i quali
il processo figlio dovrà eseguirne uno solo, che si identifica con il suo indice di argv.

L'indice, per capire quale comando eseguire, lo calcola il padre come 1 + il modulo tra il pid del processo bash (ossia il padre del main) 
e il numero dei comandi candidati ad essere eseguiti.
Tale indice lo trasmette al figlio tramite "void trasmissione_indice(int segment_id, int x);" tramite la memoria condivisa.
L'output deve essere il pid di bash e il risultato del comando con quell'indice.

Poi farlo anche con la pipe, però senza funzione ausiliare scelta_processo.

In entrambi i casi gestire gli eventuali errori generati dalle varie funzioni.


esempio:
input da riga di comando:  
./a.out ls tree pwd

Il pid di bash è: 2682
Pertanto eseguo il comando con indice: 1
"output del comando ls"*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

void trasmissione_indice(int segment_id, int x);

int main(int argc, char*argv[]){
    if(argc<=1){
        fprintf(stderr,"Inserire almeno un comando\n");
        return 1;
    }
    int segment_id=shmget(IPC_PRIVATE, sizeof(int), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della memoria dinamica\n");
        return 1;
    }
    pid_t pid=fork();
    int index;
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del processo figlio\n");
        return 1;
    }else if(pid>0){
        printf("Il pid di bash è: %d\n",getppid());        
        index=1+(getppid() % (argc-1));
        printf("Pertanto eseguo il comando %d\n",index);
        
        trasmissione_indice(segment_id, index);
        wait(NULL);
    
        
    }else{
        int*mess=(int*)shmat(segment_id,NULL,0);
        if(mess==(int*)-1){
            fprintf(stderr,"Errore nell'attach del padre\n");
            shmctl(segment_id,IPC_RMID,NULL);
            return 1;
        }
        sleep(2);
        index=*mess;
        shmdt(mess);
        
        char com[6];
        char tmp[30];
        
        strcpy(com,"/bin/");
        strcpy(tmp,argv[index]);
        strcat(com,tmp);
        
        printf("index=%d\n",index);
        printf("eseguo %s\n",com);
        execlp(com,argv[index],NULL);
        printf("Errore\n");
    }
}

void trasmissione_indice(int segment_id, int x){
    int*mess=(int*)shmat(segment_id,NULL,0);
    if(mess==(int*)-1){
        fprintf(stderr,"Errore nell'attach del padre\n");
        shmctl(segment_id,IPC_RMID,NULL);
        return;
    }
    *mess=x;
    shmdt(mess);
}
