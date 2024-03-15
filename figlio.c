#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // fork a child process
    
    //printf("Sono qui\n");
    pid = fork();
    printf("\nSono qui\n");
    //printf ("PID=%d; My parent PID=%d\n", getpid(), getppid());
    
    if(pid < 0) {
        // error occurred
        fprintf(stderr, "Fork failed\n");
    }else if (pid == 0) {
            // child process
            printf("\nCHILD process, infatti pid %d, perchè ci troviamo nell'istanza di codice del figlio\n", pid);
            printf ("child PID=%d; My parent PID=%d\n", getpid(), getppid());
            execlp("/bin/ls", "ls", NULL);
    } else {
            // parent process
            fprintf(stdout, "I am the PARENT process with PID %d\n",getpid());
            printf("Ci troviamo nell'istanza di codice del padre e il PID del figlio è esattamente %d\n",pid);
            printf("il suo parent è il programma bash e il suo PID %d\n",getppid());
            // parent process wait for the child to complete
            wait(NULL);
            fprintf(stdout, "\nChild completed\n");
    }
    fprintf(stdout, "Program completed!\n");
    
    return 0;
}
