#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int fattoriale(int);
void*factorial_thread(void* );

typedef struct{
    int n;
    int res;
}TS;

int main(){
    int n,k;
    
    do{
        printf("Inserire n e k:\n");
        scanf("%d %d",&n,&k);
    }while(n<0 || k<0 || n<k);
    
    pthread_t tid[3];
    int i;
    TS dati[3]; //tanti thread tanti parametri
    dati[0].n=n;
    dati[1].n=k;
    dati[2].n=n-k;
    
    
    for(i=0; i<3; i++){
        pthread_create(tid+i, NULL, factorial_thread, dati+i);
    }
    
    for(i=0; i<3; i++){
        pthread_join(tid[i],NULL);
        printf("Il fattoriale corrente è: %d\n",dati[i].res);
    }
    
    printf("Coefficiente binomiale: %d\n",(dati[0].res)/((dati[1].res)*(dati[2].res)) );
}

void*factorial_thread(void*tmp){
    TS*dati=(TS*)tmp;
    
    dati->res=fattoriale(dati->n);
}

int fattoriale(int x){
    if(x==0)
        return 1;
    else
        return x*fattoriale(x-1);
}
