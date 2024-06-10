#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_REINDEERS 9
static int reindeers_waiting = 0;

pthread_mutex_t mutex_santa = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_santa = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_reindeer_holiday = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reindeers = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_reindeers_flying[NUM_REINDEERS] = {PTHREAD_MUTEX_INITIALIZER};

void* reindeer_activity(void* arg) {
    int reindeer_id = *(int*)arg;
    while (1) {
        printf("Reindeer %d is on holidays\n", reindeer_id);
        sleep(5 + rand() % 6);

        pthread_mutex_lock(&mutex_reindeer_holiday);
        reindeers_waiting++;
        printf("Reindeer %d: %d waiting\n", reindeer_id, reindeers_waiting);
        if (reindeers_waiting == NUM_REINDEERS) {
            printf("Reindeer %d: waking up Santa\n", reindeer_id);
            pthread_cond_signal(&cond_santa);
        }
        pthread_mutex_unlock(&mutex_reindeer_holiday);
        pthread_mutex_lock(&mutex_reindeers_flying[reindeer_id]);
        while (reindeers_waiting > 0) {
            pthread_cond_wait(&cond_reindeers, &mutex_reindeers_flying[reindeer_id]);
        }
        pthread_mutex_unlock(&mutex_reindeers_flying[reindeer_id]);
    }
}

void* santa_activity() {
    while (1) {
        printf("Santa is sleeping\n");
        pthread_cond_wait(&cond_santa, &mutex_santa);

        printf("Santa is riding\n");
        sleep(5);
        printf("Santa has returned\n");
        reindeers_waiting = 0;
        pthread_cond_broadcast(&cond_reindeers);
    }
}

int main() {
    pthread_t santa_thread;
    pthread_create(&santa_thread, NULL, santa_activity, NULL);
    int reindeer_ids[NUM_REINDEERS];
    for (int i=0; i<NUM_REINDEERS; i++) {
        pthread_t reindeer_thread;
        reindeer_ids[i]= i;
        pthread_create(&reindeer_thread, NULL, reindeer_activity, &reindeer_ids[i]);
    }
    pthread_join(santa_thread, NULL);
    return 0;
}