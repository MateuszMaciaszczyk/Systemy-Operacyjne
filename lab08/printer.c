#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "config.h"


int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s <number of printers>\n", argv[0]);
        return -1;
    }

    int shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(shared_memory_fd, sizeof(shared_memory_t));
    shared_memory_t* shared_memory = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    int printers_number = atoi(argv[1]);
    shared_memory->printer_count = printers_number;

    for (int i = 0; i < printers_number; i++) {
        sem_init(&shared_memory->printers[i].semaphore, 1, 1);
        if (fork() == 0) {
            while (true) {
                if (shared_memory->printers[i].state == BUSY) {
                    for (int j = 0; j < shared_memory->printers[i].length; j++) {
                        printf("Printer %d is printing: %c  |  (%d / %d)\n", i, shared_memory->printers[i].buffer[j], j + 1, shared_memory->printers[i].length);
                        sleep(1);
                    }
                    shared_memory->printers[i].state = EMPTY;
                    sem_post(&shared_memory->printers[i].semaphore);
                }
            }
            exit(0);
        }
    }

    for (int i = 0; i < printers_number; i++) {
        wait(NULL);
        sem_destroy(&shared_memory->printers[i].semaphore);
    }
    munmap(shared_memory, sizeof(shared_memory_t));
    shm_unlink(SHARED_MEMORY_NAME);
    return 0;
}