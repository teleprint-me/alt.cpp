/*
 * tests/test_pthread.c
 */
#include "../include/vector.h"

#include <pthread.h>
#include <stdio.h>

void initialize_vector(struct Vector* vector, size_t offset) {
    for (size_t i = 0; i < vector->size; i++) {
        vector->elements[i] = i + offset;
    }
}

int main() {
    initialize_global_logger(LOG_LEVEL_INFO, LOG_TYPE_STREAM, "stream", stderr, NULL);

    pthread_t      thread_id;
    pthread_attr_t attr;

    struct Vector* a = vector_create(5);
    initialize_vector(a, 1);

    struct Vector* b = vector_create(5);
    initialize_vector(b, 2);

    struct Vector* c = NULL;

    vector_add(a, b);

    // Initialize thread attributes
    pthread_attr_init(&attr);

    // Create new thread
    pthread_create(&thread_id, &attr, print_hello, NULL);

    // Perform some other tasks in the main thread

    // Join the threads with the main thread
    pthread_join(thread_id, NULL);

    printf("Thread %ld has terminated.\n", (long int) thread_id);

    pthread_exit(NULL);
}
