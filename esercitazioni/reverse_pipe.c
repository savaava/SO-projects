#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 100

typedef struct{
    char str[SIZE];
}shared_data;

void reverse_str(char*str){
    int len=strlen(str);
    for(int i=0; i<len/2; i++){
        char temp=str[i];
        str[i]=str[len-i-1];
        str[len-i-1]=temp;
    }
}

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non è stato inviato un parametro!\n");
        return 1;
    }
    
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe!\n");
        return 1;
    }
    
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio!\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        printf("La stringa non invertita :%s\n",argv[1]);
        shared_data dato;
        strcpy(dato.str,argv[1]);
        reverse_str(dato.str);
        if(write(fd[1],&dato,sizeof(shared_data)) <sizeof(shared_data)){
            fprintf(stderr,"Errore nella scrittura!\n");
            return 1;
        }
        
        close(fd[1]);
        return 0;
    }else{
        close(fd[1]);
        
        shared_data dato;
        int ricevuti=read(fd[0],&dato,sizeof(shared_data));
        if(ricevuti==-1){
            fprintf(stderr,"Errore nell lettura!\n");
            return 1;
        }else if(ricevuti<sizeof(shared_data))
            printf("Messaggio parziale forse!\n");
            printf("La stringa invertita è: %s\n",dato.str);
        
            close(fd[0]);
        return 0;
    }
    
}
