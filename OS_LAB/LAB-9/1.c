#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // Number of philosophers

// States of the philosophers
enum { THINKING, HUNGRY, EATING } state[N];

// Mutex for synchronizing access to the shared variables
pthread_mutex_t mutex;

// Condition variables for each philosopher
pthread_cond_t cond_var[N];

void pickup_forks(int i);
void putdown_forks(int i);
void test(int i);
void* philosopher(void* num);

int main() {
    int i;
    pthread_t thread_id[N];
    int philosopher_num[N];

    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < N; i++) {
        pthread_cond_init(&cond_var[i], NULL);
        philosopher_num[i] = i;
    }
    // Create philosopher threads
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &philosopher_num[i]);
    }
    // Join philosopher threads
    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    for (i = 0; i < N; i++) {
        pthread_cond_destroy(&cond_var[i]);
    }
    return 0;
}
// Philosopher thread function
void* philosopher(void* num) {
    int i = *(int*)num;

    while (1) {
        printf("Philosopher %d is THINKING.\n", i);
        sleep(1);  // Philosopher is thinking

        pickup_forks(i);  // Attempt to pick up forks

        printf("Philosopher %d is EATING.\n", i);
        sleep(2);  // Philosopher is eating

        putdown_forks(i);  // Put down forks
    }
}

// Attempt to pick up forks
void pickup_forks(int i) {
    pthread_mutex_lock(&mutex);

    // Philosopher becomes hungry
    state[i] = HUNGRY;
    printf("Philosopher %d is HUNGRY.\n", i);

    // Test whether the philosopher can eat
    test(i);

    // If unable to eat, wait until condition is met
    if (state[i] != EATING) {
        pthread_cond_wait(&cond_var[i], &mutex);
     }

    pthread_mutex_unlock(&mutex);
}

// Put down forks
void putdown_forks(int i) {
    pthread_mutex_lock(&mutex);

    // Philosopher finishes eating and starts thinking
    state[i] = THINKING;
    printf("Philosopher %d is THINKING again.\n", i);

    // Test if neighbors can eat
    test((i + 4) % N);  // Left neighbor
    test((i + 1) % N);  // Right neighbor

    pthread_mutex_unlock(&mutex);
}

// Test if philosopher can eat
void test(int i) {
    // A philosopher can eat if they are hungry and neither neighbor is eating
    if (state[i] == HUNGRY &&
        state[(i + 4) % N] != EATING &&
        state[(i + 1) % N] != EATING) {

        // Philosopher starts eating
        state[i] = EATING;
        pthread_cond_signal(&cond_var[i]);
    }
}
