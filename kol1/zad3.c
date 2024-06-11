#include "zad3.h"

void readwrite(int pd, size_t block_size);

void createpipe(size_t block_size)
{
    /* Utwórz potok nienazwany */

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
    }

    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(fd);
    check_write(fd, block_size, readwrite);
}

void readwrite(int write_pd, size_t block_size)
{
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/

    FILE *file = fopen("unix.txt", "r");
    if (file == NULL) {
        printf("Error file\n");
        return;
    }

    char buffer[block_size];
    size_t n;
    while ((n = fread(buffer, sizeof(char), block_size, file)) > 0) {
        if (write(write_pd, buffer, n) == -1) {
            perror("write");
        }
    }

    fclose(file);
    /* Zamknij plik */
}

int main()
{
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}