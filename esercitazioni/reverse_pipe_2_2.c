#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 100

typedef struct {
    char str[SIZE];
} shared_data;

void reverse_str(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int is_palindrome(char *str1, char *str2) {
    if (strcmp(str1, str2) == 0)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[]) {
    shared_data data;

    if (argc != 2) {
        printf("Please provide a string as a command line argument!\n");
        return EXIT_FAILURE;
    } else
        strncpy(data.str, argv[1], SIZE);

    int fd1[2], fd2[2];
    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("Pipe creation error\n");
        return EXIT_FAILURE;
    }

    write(fd1[1], &data, sizeof(data));
    close(fd1[1]); // close the write end of the pipe in the parent process

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork error\n");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // First child process
        shared_data buffer1;
        read(fd1[0], &buffer1, sizeof(buffer1));
        close(fd1[0]); // close the read end of the pipe in the first child process
        reverse_str(buffer1.str);
        write(fd2[1], &buffer1, sizeof(buffer1));
        close(fd2[1]); // close the write end of the pipe in the first child process

        pid_t pid2 = fork();
        if (pid2 < 0) {
            fprintf(stderr, "Fork error\n");
            return EXIT_FAILURE;
        } else if (pid2 == 0) {
            // Second child process
            shared_data buffer2;
            read(fd2[0], &buffer2, sizeof(buffer2));
            close(fd2[0]); // close the read end of the pipe in the second child process
            printf("Original string: %s\n", data.str);
            printf("Reversed string: %s\n", buffer2.str);
            if (is_palindrome(data.str, buffer2.str))
                printf("The string is palindromes.\n");
            else
                printf("The string is not a palindrome.\n");
        }
    } else {
        // Parent process
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
