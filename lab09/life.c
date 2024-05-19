#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void signal_handler(int signo) {}

typedef struct {
    char** foreground;
    char** background;
    int start;
    int end;
} thread_data;

void* worker_thread(void* args)
{
    thread_data* data = (thread_data*)args;
    while (true)
    {
        pause();
        for (int i = data->start; i < data->end; i++)
        {
            int row = i / grid_width;
            int col = i % grid_width;
            (*data->background)[i] = is_alive(row, col, *data->foreground);
        }
    }
}


int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <number of threads>\n", argv[0]);
        return -1;
    }

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    struct sigaction sigact;
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1, &sigact, NULL);

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    thread_data data[num_threads];
    int cells_per_thread = (grid_width * grid_height / num_threads) + 1;

    for (int i = 0; i < num_threads; i++)
    {
        data[i].start = i * cells_per_thread;
        data[i].end = (i == num_threads - 1) ? (grid_width * grid_height) : ((i + 1) * cells_per_thread);
        data[i].foreground = &foreground;
        data[i].background = &background;
        pthread_create(&threads[i], NULL, worker_thread, &data[i]);
    }

    init_grid(foreground);

    while (true)
    {
        draw_grid(foreground);
        for (int i = 0; i < num_threads; i++)
        {
            pthread_kill(threads[i], SIGUSR1);
        }

        usleep(500 * 1000);

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}