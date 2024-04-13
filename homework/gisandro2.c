/*Il programma crea un processo figlio; il processo padre legge da tastiera 3 numeri reali a, b, e c e li
trasmette al processo figlio tramite una pipe; il processo figlio calcola le radici dell’equazione di secondo
grado
ax2 + bx + c = 0
usando la funzione Calcola_radici e le stanmpa a video. Il processo padre deve aspettare la terminazione
del processo figlio e stampare un messaggio quando avviene.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// Calcola le radici di un'equazione di secondo grado
void Calcola_radici(float x[]);

int main(){
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Errore nella creazione della pipe anonima\n");
        return 1;
    }
    float coeff[3];
    pid_t pid=fork();
    
    if(pid<0){
        fprintf(stderr,"Errore nella fork\n");
        return 1;
    }else if(pid==0){
        close(fd[1]);
        
        if(read(fd[0],coeff,sizeof(coeff)) <sizeof(coeff)){
            fprintf(stderr,"Errore nella ricezione\n");
            close(fd[0]);
            return 1;
        }
        Calcola_radici(coeff);
        
        close(fd[0]);
    }else{   
        close(fd[0]);        
        
        printf("Inserire i tre coefficienti: \na=");
        scanf("%f",coeff);
        printf("b=");
        scanf("%f",coeff+1);
        printf("c=");
        scanf("%f",coeff+2);
        
        if(write(fd[1], coeff, sizeof(coeff)) <sizeof(coeff)){
              fprintf(stderr,"Errore nella trasmissione\n");
              close(fd[1]);
              return 1;
        }
        
        printf("Il figlio ha terminato\n");
        
        close(fd[1]);
    }
}

void Calcola_radici(float x[]) {
    float a = x[0];
    float b = x[1];
    float c = x[2];
    float delta = b * b - 4 * a * c;
    if (delta == 0) {
    /* radici coincidenti */
        printf("radici coincidenti: x0 = x1 = %f\n", -b / (2 * a));
    } else {
        if (delta > 0) {
        /* radici reali distinte */
            printf("radici reali distinte: x0 = %f, x1 = %f\n",(-b + sqrt(delta)) / (2 * a), (-b - sqrt(delta)) / (2 * a));
        } else {
        /* radici complesse */
            printf("radici complesse coniugate: \nparte reale = %f, parte immaginaria = +/- %f\n",-b / (2 * a), sqrt(-delta) / 2 * a);
        }   
    }
}
