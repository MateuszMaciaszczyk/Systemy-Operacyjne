#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "config.h"


void rand_str(char *dest, int* length) {
    *length = rand() % 10 + 1;
    for (int i = 0; i < *length; i++) {
        dest[i] = 'a' + rand() % 26;
    }
    dest[*length] = '\0';
}


int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s <number of users>\n", argv[0]);
        return -1;
    }

    int users_num = atoi(argv[1]);

    int shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);
    ftruncate(shared_memory_fd, sizeof(shared_memory_t));
    shared_memory_t* shared_memory = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    char buffer[11];
    int length = 0;
    for (int i = 0; i < users_num; i++) {
        if (fork() == 0) {
            srand(getpid());
            while (true) {
                int printer_index = -1;
                for (int j = 0; j < shared_memory->printer_count; j++) {
                    int sem_value;
                    sem_getvalue(&shared_memory->printers[j].semaphore, &sem_value);
                    if (sem_value == 1) {
                        printer_index = j;
                        break;
                    }
                }
                if (printer_index == -1) {
                    printer_index = rand() % shared_memory->printer_count;
                }
                sem_wait(&shared_memory->printers[printer_index].semaphore);

                rand_str(buffer, &length);
                printf("User %d is writing to printer %d: %s\n", i, printer_index, buffer);
                shared_memory->printers[printer_index].length = length;
                strcpy(shared_memory->printers[printer_index].buffer, buffer);

                shared_memory->printers[printer_index].state = BUSY;
                sleep(rand()% 10 + 1);
            }
            exit(0);
        }
    }
    while (wait(NULL) > 0);
    munmap(shared_memory, sizeof(shared_memory_t));
    return 0;
}