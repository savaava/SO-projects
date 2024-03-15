#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    pid_t pid;
    unsigned int tP = 5;
    unsigned int tC = 2;

    printf ("Main: ");
    printf ("PID=%d; My parent PID=%d\n", getpid(), getppid());

    // create the child process
    pid = fork();
    if (pid < 0) {
        // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else
        if(pid == 0) {
            // child process
            sleep(tC);
            printf("sono qui, il figlio\n");
            printf("Child: PID returned=%d ", pid);
            printf("Child PID=%d; My parent PID=%d\n",getpid(), getppid());

        } else {
            // parent process
            //wait(NULL);
            printf("sono qui, il padre\n");
            sleep(tP);
            printf("sono qui, il padre è tornato\n");
            printf("Father: PID returned=%d ", pid);
            printf("Father PID=%d; My parent PID=%d\n", getpid(), getppid());
        }
    return(EXIT_SUCCESS);
}
