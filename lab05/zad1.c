#define _POSIX_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void handle_sigusr1(int sig) {
    printf("Received SIGUSR1\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <none|ignore|handler|mask>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, handle_sigusr1);
    }
    else if (strcmp(argv[1], "mask") == 0) {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newmask, NULL);
    }

    raise(SIGUSR1);

    if(strcmp(argv[1], "mask") == 0) {
        sigset_t pending;
        sigpending(&pending);
        if (sigismember(&pending, SIGUSR1)) {
            printf("SIGUSR1 is pending\n");
        }
        else {
            printf("SIGUSR1 is not pending\n");
        }
    }

    return 0;
}