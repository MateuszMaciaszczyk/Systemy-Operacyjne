#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory path>\n", argv[0]);
        return -1;
    }

    printf("%s\n\n", argv[0]);
    int local = 0;
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error: fork failed\n");
        return -1;
    } else if (pid == 0) {
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n\n", local, global);
        int exec_status = execl("/bin/ls", argv[1]);
        if (exec_status == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("\nparent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), pid);
        printf("Child exit code: %d\n", WEXITSTATUS(status));
        printf("Parent's local = %d, parent's global = %d\n", local, global);
    }

    return 0;
}