#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i;

    printf ("Main: ");
    printf ("PID=%d; My parent PID=%d\n", getpid(), getppid());

    for(i = 0; i < 2; i++) {
        // fork a child process
        pid = fork();
        if (pid < 0) {
            // error occurred
            fprintf(stderr, "Fork Failed\n");
            return EXIT_FAILURE;
        }
        else
            if (pid == 0) {
                // child process
                printf("Child : PID returned=%d     ", pid);
                printf("Child PID=%d; My parent PID=%d\n", getpid(), getppid());
                //execlp("/bin/ls", "ls", "-al", NULL);
                //printf("I am the line after executing directory listing: ls - al\n");
            } else {
                // parent process
                wait(NULL);
                printf("Father: PID returned=%d ", pid);
                printf("Father PID=%d; My parent PID=%d\n", getpid(), getppid());
                //execlp("/bin/ls", "ls", "-al", NULL);
                //printf("I am the line after executing directory listing: ls - al\n");
            }
    }
    return EXIT_SUCCESS;
}
