#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* indice(void* arg);

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non sono stati passati due stringhe da riga di comando!\n");
        return 1;
    }
    
    int n=atoi(argv[1]);
    pthread_t tid[n];   //se era solo un tid nel for devo fare poi la join dentro al for
    int i,tmp[n];
    
    /*10 core per lo scheduling ogni core col proprio thread se lancio 
    i threads contemporaneamente, ossia creo prima tutti i thread e poi lo scheduling li mette su tutti i core e poi si mette in attesa. ma se facessi join dentro il for allora è inutile non sfrutta 
    i threads, perchè aspetterei ogni volta che il singolo thread finisca.*/
    for(i=0; i<n; i++){
        tmp[i]=i;
        if(pthread_create(tid+i,NULL,indice,tmp+i) !=0){
            fprintf(stderr,"errore nella creazione del thread figlio!\n");
            return 2;
        }
    }
    
    for(i=0; i<n; i++)
        pthread_join(tid[i],NULL);
    
    
    return 0;
}

void* indice(void* arg){
    int ind=*((int*)arg);
    printf("Sono il thread con indice %d\n",ind);
  
    pthread_exit(NULL);
}
