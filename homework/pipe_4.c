/*๏	Scrivere un programma che crea una pipe anonima, dopodiché crea un processo ﬁglio
๏	Il processo ﬁglio legge 5 numeri interi da tastiera e li invia al processo padre tramite la pipe, mandando i primi 2, aspettando 1 secondo e quindi mandando gli ultimi 3
๏	Il processo padre legge dalla pipe i 5 numeri inviati dal ﬁglio e li stampa
๏	Nella lettura, gestire il fatto che il messaggio sia incompleto ripetendo l’operazione di lettura ﬁnché il messaggio non viene completato, oppure si veriﬁca un errore che impedisce il completamento*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 5

int leggi_messaggio(int fd,int*msg,int size);

int main(){
    int fd[2];
    int i,inviati,ricevuti,tot=0;
    int messaggio[5];
    
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe\n");
        return 1;
    }
    
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        printf("Inserire 5 interi:\n");
        for(i=0;i<N;i++){
            printf("Inserire mess[%d]=",i);
            scanf("%d",messaggio+i);
        }
        inviati=write(fd[1],messaggio,2*sizeof(int));
        if(inviati<2*sizeof(int)){
            fprintf(stderr,"Errore nell'invio 1\n");
            return 1;
        }
        printf("\nIl figlio dorme per 1 secondo\n\n");
        sleep(1);
        printf("Il figlio scrive i restanti 3\n");
        inviati=write(fd[1],messaggio+2,3*sizeof(int));
        if(inviati<3*sizeof(int)){
            fprintf(stderr,"Errore nell'invio 2\n");
            return 1;
        }
        close(fd[1]);
        return 0;
    }else{
        close(fd[1]);
        
        ricevuti=leggi_messaggio(fd[0],messaggio,N);
        
        if(ricevuti==-1){
            printf("Errore\n");
            return 1;
        }
        if(ricevuti<N)
            printf("Ne sono arrivati di meno\n");
        else{
            printf("\nSono arrivati tutti e 5\n");
            for(i=0;i<N;i++)
                printf("mes[%d]=%d\n",i,messaggio[i]);
        }
        close(fd[0]);
        return 0;
    }
}

int leggi_messaggio(int fd,int*msg,int size){
    int tot=0,ricevuti;
    
    ricevuti=read(fd,msg+tot,2*sizeof(int));
    if(ricevuti==-1){
        printf("Errore\n");
        return 1;
    }
    tot+=ricevuti/sizeof(int);
    printf("Ho ricevuto i primi %d\n",tot);
    ricevuti=read(fd,msg+tot,3*sizeof(int));
    if(ricevuti==-1){
        printf("Errore\n");
        return 1;
    }
    tot+=ricevuti/sizeof(int);
    printf("Adesso invece ne ho ricevuti %d\n",tot);
      
    return tot;
}
