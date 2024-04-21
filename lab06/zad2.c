#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <start> <end>\n", argv[0]);
        return -1;
    }

    double start = atof(argv[1]);
    double end = atof(argv[2]);

    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    int fd = open(myfifo, O_WRONLY);
    write(fd, &start, sizeof(double));
    write(fd, &end, sizeof(double));
    close(fd);

    char * myfifo2 = "/tmp/myfifo2";
    int fd2 = open(myfifo2, O_RDONLY);

    double result;
    read(fd2, &result, sizeof(double));
    close(fd2);

    printf("Integral value: %f\n", result);

    return 0;
}