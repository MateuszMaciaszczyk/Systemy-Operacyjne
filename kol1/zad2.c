#include "zad2.h"

void mask()
{
    /*  Zamaskuj sygnał SIGUSR2, tak aby nie docierał on do procesu */
    sigset_t mask_set;
    sigemptyset(&mask_set);
    sigaddset(&mask_set, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &mask_set, NULL) == -1) {
        perror("sigprocmask");
    }

    check_mask();
}

void process()
{
    /*  Stworz nowy process potomny i uruchom w nim program ./check_fork
        W procesie macierzystym:
            1. poczekaj 1 sekundę
            2. wyślij SIGUSR1 do procesu potomnego
            3. poczekaj na zakończenie procesu potomnego */

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid == 0) {
        execl("./check_fork", "check_fork", NULL);
        perror("execl");
    }

    sleep(1);
    kill(pid, SIGUSR1);
    wait(NULL);
}

int main()
{
    mask();
    process();

    return 0;
}