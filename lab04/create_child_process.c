#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of child processes>\n", argv[0]);
        return -1;
    }

    int num_children = atoi(argv[1]);

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Error: fork failed\n");
            return -1;
        } else if (pid == 0) {
            printf("Parent PID: %d, Child PID: %d\n", getppid(), getpid());
            exit(0);
        }
    }

    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    printf("%s\n", argv[1]);

    return 0;
}