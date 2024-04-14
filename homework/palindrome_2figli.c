/*Scrivere un programma in linguaggio C chiamato reverse.c che inverte una stringa.
Il programma deve prendere in ingresso da linea comando la stringa da invertire:
./reverse <string>
e deve stampare a video la stringa invertita. Si usi la struttura dati seguente per la
comunicazione:
typedef struct {
char str[SIZE];
} shared_data;
Scrivere una soluzione basata su shared memory, nella quale il processo figlio
effettua l’inversione ed il processo padre provvede alla stampa a video.

controllando se la stringa è
palindroma confrontando la stringa originale e quella invertita.
Scrivere una soluzione basta su shared memory nella quale il processo
figlio crea un ulteriore processo per il confronto delle stringhe originale e
invertita.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 100

typedef struct {
    char str[SIZE];
} shared_data;

void reverse_str(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    if(argc!=2){
        fprintf(stderr,"Inserire 1 stringa da riga di comando!\n");
        return 1;
    }
    
    int segment_id=shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR|S_IWUSR);
    if(segment_id==-1){
        fprintf(stderr,"Errore nell'allocazione della memoria condivisa!\n");
        return 1;
    }
    
    shared_data*mess; //sarà il messaggio che si scambia il figlio col padre
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella fork!\n");
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    }else if(pid==0){ //figlio
        mess=(shared_data*)shmat(segment_id, NULL, 0);
        if(mess==(shared_data*)-1){
            fprintf(stderr,"Errore nel collegamento alla mem!\n");
            return 1;
        }
        
        printf("Sono il figlio e la stringa è: %s\n",argv[1]);
        strcpy(mess->str,argv[1]);
        reverse_str(mess->str);
        
        pid=fork();
        if(pid<0){
            fprintf(stderr,"Errore nella fork!\n");
            return 1;
        }else if(pid==0){ //nipote
            if(strcmp(argv[1],mess->str)==0)
                printf("La stringa inserita da riga di comando è palindroma!\n");
            else
                printf("La stringa inserita da riga di comando non è palindroma!\n");
            shmdt(mess); //il nipote aveva il puntatore mess già collegato all'area di mem, pertanto non avevamo bisogno della shmat, ma dobbiamo comunque fare shmdt
        }else{
            wait(NULL);
        }
        
        shmdt(mess);
    }else{ //padre
        mess=(shared_data*)shmat(segment_id, NULL, 0);
        if(mess==(shared_data*)-1){
            fprintf(stderr,"Errore nel collegamento alla mem!\n");
            shmctl(segment_id, IPC_RMID, NULL);
            return 1;
        }
        
        wait(NULL);
        
        printf("Sono il padre e la stringa intertita è: %s\n",mess->str);
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
    }
    
    return 0;
}










