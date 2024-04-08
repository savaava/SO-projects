/**************************************************************
Il candidato completi il programma fornito, implementando
il main. Il programma ottiene un numero intero N dai parametri
della linea di comando, e crea un processo figlio;
il processo figlio deve calcolare il quadrato, il cubo, e la
quarta potenza di N e deve passare questi tre valori
al processo padre; il processo padre deve attendere la terminazione del figlio
e poi stampare i tre valori ricevuti.
La comunicazione tra processo padre e processo
figlio deve avvenire tramite l'uso di shm.
Il candidato può definire, se lo ritiene utile, funzioni ausiliarie.

Una volta collegati, premere il pulsante FORK THIS di fianco al pulsante
RUN in alto a sinistra.
Inserire nella finestra principale il codice sorgente completo,
inserire i parametri da passare sulla linea comando nella casella CommandLine Arguments
e cliccare sul pulsante RUN per visualizzare il risultato dell'esecuzione.
Utilizzare Interactive Console come standard input per inserire valori da tastiera.

*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h> 

#define n 3

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non è arrivato un nuomero da riga di comando!\n");
        return 1;
    }
    
    int segment_id=shmget(IPC_PRIVATE, n*sizeof(int), S_IRUSR | S_IWUSR);
    if(segment_id<0){
        fprintf(stderr,"Errore nell'allocazione della memoria condivisa!\n");
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio!\n");
        return 1;
    }else if(pid==0){
        int tmp=atoi(argv[1]);
        //int*mess=(int*)malloc(n*sizeof(int));
        int*mess=(int*)shmat(segment_id, NULL, 0);
        
        mess[0]=tmp*tmp;
        mess[1]=tmp*tmp*tmp;
        mess[2]=tmp*tmp*tmp*tmp;
        
        shmdt(mess);
        return 0;
    }else{
        wait(NULL);
        
        int*mess=(int*)shmat(segment_id, NULL, 0);
        printf("Il numero è %d\nil suo quadrato: %d\nil suo cubo: %d\nla sua quarta potenza: %d\n",atoi(argv[1]),mess[0],mess[1],mess[2]);
        
        shmdt(mess);
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}
