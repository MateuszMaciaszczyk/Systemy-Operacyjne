#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


double calculate_integral(double start, double end, double width) {
    double sum = 0;
    for(double x = start; x < end; x += width) {
        sum += 4.0 / (1.0 + x * x) * width;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <width> <processes>\n", argv[0]);
        return -1;
    }

    double width = atof(argv[1]);
    int processes = atoi(argv[2]);
    double interval_width = 1.0 / processes;

    if(processes > (1.0 / width)) {
        printf("Error: The number of processes cannot be greater than the number of rectangles (%d).\n", (int)(1 / width));
        return -1;
    }

    int fds[processes][2];
    for(int i = 0; i < processes; i++) {
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

            double start = i * interval_width;
            double end = start + interval_width;

            clock_t start_time = clock();
            double result = calculate_integral(start, end, width);
            clock_t end_time = clock();

            double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            printf("Execution time: %f\n", time_spent);

            FILE *file = fopen("report.txt", "a");

            if(file == NULL) {
                perror("fopen");
                return -1;
            }
            fprintf(file, "Execution time for precision: %f and process %d/%d: %f\n", width, i, processes, time_spent);
            fclose(file);

            write(fds[i][1], &result, sizeof(double));
            close(fds[i][1]);

            return 0;
        } else {
            close(fds[i][1]);
        }
    }

    double sum = 0;
    for(int i = 0; i < processes; i++) {
        double result;
        read(fds[i][0], &result, sizeof(double));
        close(fds[i][0]);
        sum += result;
    }

    printf("Integral value: %f\n", sum);

    return 0;
}