#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

double calculate_integral(double start, double end, double width) {
    double sum = 0;
    for(double x = start; x < end; x += width) {
        sum += 4.0 / (1.0 + x * x) * width;
    }
    return sum;
}

int main() {
    double start, end;

    char * myfifo = "/tmp/myfifo";
    int fd = open(myfifo, O_RDONLY);

    read(fd, &start, sizeof(double));
    read(fd, &end, sizeof(double));
    close(fd);

    double interval = (end - start) / 100;
    int fds[100][2];

    for(int i = 0; i < 100; i++) {
        if(pipe(fds[i]) == -1) {
            perror("pipe");
            return -1;
        }

        pid_t pid = fork();
        if(pid == -1) {
            perror("fork");
            return -1;
        } else if(pid == 0) {
            close(fds[i][0]);

            double local_start = start + i * interval;
            double local_end = local_start + interval;

            double result = calculate_integral(local_start, local_end, 0.0001);

            write(fds[i][1], &result, sizeof(double));
            close(fds[i][1]);

            return 0;
        } else {
            close(fds[i][1]);
        }
    }

    double sum = 0;
    for(int i = 0; i < 100; i++) {
        double result;
        read(fds[i][0], &result, sizeof(double));
        close(fds[i][0]);
        sum += result;
    }

    char * myfifo2 = "/tmp/myfifo2";
    mkfifo(myfifo2, 0666);

    int fd2 = open(myfifo2, O_WRONLY);
    write(fd2, &sum, sizeof(double));
    close(fd2);

    return 0;
}