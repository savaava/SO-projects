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
    
    pid_t pid=fork();
    int sh_id=shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR|S_IWUSR);
    
    if(pid<0){
        fprintf(stderr,"Errore nella creazione del figlio!\n");
        return 1;
    }else if(pid==0){
        shared_data*s=(shared_data*)shmat(sh_id, NULL, 0);
        printf("La stringa passata da riga di comando è: %s\n",argv[1]);
        strcpy(s->str,argv[1]);
        reverse_str(s->str);
        printf("La stringa invertita nel figlio : %s\n",s->str);
        shmdt(s);
        return 0;
    }else{
        shared_data*s=(shared_data*)shmat(sh_id, NULL, 0);
        wait(NULL);
        
        printf("La stringa invertita nel padre è: %s\n",s->str);
        
        shmdt(s);
        shmctl(sh_id,IPC_RMID,NULL);
        return 0;
    }
    
}
