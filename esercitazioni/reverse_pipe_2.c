#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int is_palindrome(char*str1,char*str2){
    return strcmp(str1,str2) ? 0:1;
}

int main(int argc, char*argv[]){
    if(argc!=2){
        fprintf(stderr,"Non è stato passato 1 stringa da riga di comando!\n");
        return 1;
    }
    
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe!\n");
        return 1;
    }
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del processo figlio!\n");
        return 1;
    }else if(pid==0){
        close(fd[0]);
        
        printf("La stringa non invertita dal figlio è: %s\n",argv[1]);
        shared_data mess;
        strcpy(mess.str,argv[1]);
        reverse_str(mess.str);
        if(write(fd[1], &mess, sizeof(shared_data)) <sizeof(shared_data)){
            fprintf(stderr,"Errore nella scrittura sulla pipe!\n");
            return 1;
        }
        
        int fd_2[2];
        if(pipe(fd_2)==-1){
            fprintf(stderr,"Errore nella creazione della pipe\n");
            return 1;
        }        
        
        pid_t pid2=fork();
        if(pid2<0){
            fprintf(stderr,"Errore nella creazione del processo figlio!\n");
            return 1;
        }else if(pid2==0){
            close(fd_2[1]);
            
            shared_data mess;
            int ricevuti=read(fd_2[0], &mess, sizeof(shared_data));
            
            if(ricevuti==-1){
                fprintf(stderr,"Errore nella ricezione del messaggio!\n");
            return 1;
            }else if(ricevuti <sizeof(shared_data))
                printf("Messaggio parziale nel figlio del figlio\n");
                
            printf("Siamo nel figlio del figlio e\n");
            if(is_palindrome(mess.str,argv[1]))
                printf("La stringa %s è palindroma\n",mess.str);
            else
                printf("La stringa %s non è palindroma\n",mess.str);
                
            close(fd_2[0]);
            return 0;
        }else{
            close(fd_2[0]);
            if(write(fd_2[1], &mess, sizeof(shared_data)) <sizeof(shared_data)){
                fprintf(stderr,"Errore nella scrittura sulla pipe!\n");
                return 1;
            }
            close(fd_2[1]);
            return 0;
        }
        
        close(fd[1]);
        return 0;
    }else{
        close(fd[1]);
        
        shared_data mess;
        int ricevuti=read(fd[0], &mess, sizeof(shared_data));
        if(ricevuti==-1){
            fprintf(stderr,"Errore nella ricezione del messaggio!\n");
            return 1;
        }else if(ricevuti <sizeof(shared_data))
            printf("Messaggio parziale!\n");
            
        printf("La stringa invertita dal padre è: %s\n",mess.str);
        
        close(fd[0]);
        return 0;
    }
}
