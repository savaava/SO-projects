/*padre genera due figli suoi
2 numeri interi scritti da riga di comando,
Un figlio effettua la somma dei due numeri e l'altro il prodotto 
la rimandano tramite shm al padre che calcola la somma dei due risultati
e lo stampa su output*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, char*argv[]){
    if(argc!=3){
        fprintf(stderr,"Inserire due numeri\n");
        return 1;
    }
    
    int*mess;
    int n1=atoi(argv[1]);
    int n2=atoi(argv[2]);
    if(n1<=0 || n2<=0){
        fprintf(stderr,"Inserire due numeri interi\n");
        return 1;
    }
    
    int segment_id=shmget(IPC_PRIVATE, 2*sizeof(int), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della mem condivisa\n");
        return 1;
    }
    
    pid_t pid1=fork();
    pid_t pid2=fork();  
    
    if(pid1<0 || pid2<0){
        fprintf(stderr,"Errore nella fork");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }else if(pid1==0 && pid2==0){ //è il figlio del figlio 1, per cui non mi serve a niente
    }else if(pid1==0 && pid2>0){
        printf("SOno figlio 1\n");
        mess=(int*)shmat(segment_id, NULL, 0);
        mess[0]=n1+n2;
        shmdt(mess);
    }else if(pid1>0 && pid2==0){
        printf("SOno figlio 2\n");
        mess=(int*)shmat(segment_id, NULL, 0);
        mess[1]=n1*n2;
        shmdt(mess);
    }else{   
        mess=(int*)shmat(segment_id, NULL, 0);
        
        sleep(2);
        
        printf("la somma=%d + il prodotto=%d è:\n%d\n",mess[0],mess[1],mess[0]+mess[1]);
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
    }
}
