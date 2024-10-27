#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100;  // Producing an item (random number)

        while (count == BUFFER_SIZE) {
            // Buffer is full, producer sleeps (wait)
            printf("BUFFER IS FULL SO PRODUCER UNABLE TO PRODUCE\n\n");
            sleep(1);
        }

        // Produce item and add to buffer
        buffer[count] = item;
        count++;
        printf("Produced: %d, Items in buffer: %d\n", item, count);

        // sleep(1);  // Simulate time delay for producing an item
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    while (1) {

        while (count == 0) {
            // Buffer is empty, consumer sleeps (wait)
            printf("BUFFER IS EMPTY SO CONSUMER UNABLE TO CONSUME\n\n");
            sleep(1);
        }

        // Consume item from buffer
        item = buffer[--count];
        printf("Consumed: %d, Items in buffer: %d\n", item, count);

        // sleep(1);  // Simulate time delay for consuming an item
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads (optional, to wait for them to finish)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
