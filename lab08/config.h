#ifndef LABY_SYSOPY_CONFIG_H
#define LABY_SYSOPY_CONFIG_H
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SHARED_MEMORY_NAME "/shared_memory"

typedef enum {
    EMPTY = 0,
    BUSY = 1
} state_t;

typedef struct {
    sem_t semaphore;
    state_t state;
    int length;
    char buffer[11];
} printer_t;

typedef struct {
    printer_t printers[10];
    int printer_count;
} shared_memory_t;

#endif
