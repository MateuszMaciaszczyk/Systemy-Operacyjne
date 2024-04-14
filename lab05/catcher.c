#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int mode = 0;
int change_requests = 0;

void handle_sigusr1(int sig, siginfo_t *info, void *ucontext) {
    change_requests++;
    mode = info->si_int;
    kill(info->si_pid, SIGUSR1);
}

int main() {
    printf("Catcher PID: %d\n", getpid());

    struct sigaction act;
    act.sa_sigaction = handle_sigusr1;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);

    while(1) {
        pause();
        if(mode == 1) {
            for(int i = 1; i <= 100; i++) {
                printf("%d\n", i);
            }
        } else if(mode == 2) {
            printf("Number of change requests: %d\n", change_requests);
        } else if(mode == 3) {
            exit(0);
        }
    }

    return 0;
}