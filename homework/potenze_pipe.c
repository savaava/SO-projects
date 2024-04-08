/**************************************************************
Il candidato completi il programma fornito, implementando
il main. Il programma ottiene un numero intero N dai parametri
della linea di comando, e crea un processo figlio;
il processo figlio deve calcolare il quadrato, il cubo, e la
quarta potenza di N e deve passare questi tre valori
al processo padre; il processo padre deve attendere la terminazione del figlio
e poi stampare i tre valori ricevuti.
La comunicazione tra processo padre e processo
figlio deve avvenire tramite l'uso di pipe.
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
#include <unistd.h>

#define n 3

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non è arrivato un intero da riga di comando!\n");
        return 1;
    }
    
    int fd[2];
    if(pipe(fd)!=0){
        fprintf(stderr,"Errore nella creazione della pipe!\n");
        return 1;
    }    
    //int mess[3];
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figli!\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        int mess[n];
        int tmp=atoi(argv[1]);
        
        mess[0]=tmp*tmp;
        mess[1]=tmp*tmp*tmp;
        mess[2]=tmp*tmp*tmp*tmp;
        
        if(write(fd[1],mess,n*sizeof(int)) <0){
            fprintf(stderr,"Errore nella scrittura nella pipe!\n");
            return 1;
        }
        
        close(fd[1]);
        return 0;
    }else{
        close(fd[1]);
        
        int mess[n];
        int ricevuti=read(fd[0],mess,n*sizeof(int));
        
        if(ricevuti==-1){
            fprintf(stderr,"Errore nella lettura della pipe!\n");
            return 1;
        }else if(ricevuti<n*sizeof(int))
            printf("Messaggio parziale!\n");
            
        printf("Il numero è %d\nil suo quadrato: %d\nil suo cubo: %d\nla sua quarta potenza: %d\n",atoi(argv[1]),mess[0],mess[1],mess[2]);
        
        close(fd[0]);
        return 0;
    }    
}
