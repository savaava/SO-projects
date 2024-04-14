/*Scrivere un programma che crea una pipe anonima, dopodiché crea un processo figlio. Il processo
figlio legge 5 numeri interi da tastiera e li invia al processo padre tramite la pipe, mandando i primi 2,
aspettando 1 secondo e quindi mandando gli ultimi 3. Il processo padre legge dalla pipe i 5 numeri
inviati dal processo figlio e li stampa. Nella lettura dalla pipe, gestire circostanza che il messaggio
possa essere incompleto ripetendo l’operazione di lettura finché il messaggio non viene completato,
oppure si verifica un errore che impedisce il completamento.
Nota: puó essere d’aiuto definire una funzione ausiliaria che effettua la lettura dalla pipe, gestendo
il completamento del messaggio.
Nota bene: anche se il messaggio in questo caso è formato da interi, dal punto di vista della pipe il
messaggio è una sequenza di byte. La funzione di lettura deve tenerne conto per gestire correttamente
il completamento del messaggio.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 5

int main(){
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe\n");
        return 1;
    }
    
    int mess[5];
    int i;
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella fork\n");
        //non c'è bisogno di chiudere i fd perchè lo farà il so
        return 1;
    }else if(pid==0){
        close(fd[0]); //perchè il figlio deve solo scrivere al padre
        
        for(i=0; i<n; i++){
            printf("Scrivere il numero %d: ",i);
            scanf("%d",mess+i);
        }
        
        printf("\nIl figlio trasmette i numeri 1, 2: (%d, %d)...\n",mess[0],mess[1]);
        if(write(fd[1], mess, 2*sizeof(int)) <2*sizeof(int)){ //mando i primi due numeri interi
            fprintf(stderr,"Errore nella trasmissione del messaggio\n");
            return 1;
        }
        printf("Il figlio ha trasmesso nella pipe i primi due numeri, pertando aspetta 1 secondo\n");
        sleep(1);
        printf("Il figlio trasmette gli ultimi numeri 3, 4, 5: (%d, %d, %d)...\n",mess[2],mess[3],mess[4]);
        if(write(fd[1], mess+2, 3*sizeof(int)) <3*sizeof(int)){ //mando gli ultimi tre numeri interi
            fprintf(stderr,"Errore nella trasmissione del messaggio\n");
            return 1;
        }
        
        close(fd[1]);
    }else{
        close(fd[1]); //perchè il padre deve solo leggere dal figlio
        
        i=0;
        do{
            if(read(fd[0], mess+i, sizeof(mess[i])) <sizeof(mess[i]))
                printf("Messaggio non ricevuto correttamente (arrivato parzialmente o con errore)\nsi ripete la lettura\n");
            else{
                printf("messaggio numero %d: %d ricevuto correttamente dal padre\n",i+1,mess[i]);
                i++;
            }
        }while(i<n);
        
        close(fd[0]);
    }
}
