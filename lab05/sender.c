#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sigusr1(int sig) {
    exit(0);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <catcher_pid> <mode>\n", argv[0]);
        return -1;
    }

    pid_t catcher_pid = atoi(argv[1]);
    int mode = atoi(argv[2]);

    signal(SIGUSR1, handle_sigusr1);

//    union sigval value;
//    value.sival_int = mode;
    //sigqueue(catcher_pid, SIGUSR1, value);

    sigqueue(catcher_pid, SIGUSR1, (union sigval) {mode}); // this

    pause();

    return 0;
}