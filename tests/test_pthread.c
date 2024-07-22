/*
 * tests/test_pthread.c
 */
#include "../include/logger.h"
#include "../include/vector.h"

#include <pthread.h>
#include <stdio.h>

struct Vector* initialize_vector(size_t size, size_t offset, (float) (*operation)(float, float) ) {
    struct Vector* vector = vector_create(size);

    for (size_t i = 0; i < vector->size; i++) {
        vector->elements[i] = operation(i, offset);
    }

    return vector;
}

int main() {
    initialize_global_logger(LOG_LEVEL_INFO, LOG_TYPE_STREAM, "stream", stderr, NULL);

    pthread_t      thread_id;
    pthread_attr_t attr;

    size_t size = 5; // Vectors must be the same size.

    struct Vector* a = initialize_vector(size, 1, add);
    struct Vector* b = initialize_vector(size, 2, add);
    struct Vector* c = NULL;

    struct Vector* x = initialize_vector(size, 1, multiply);
    struct Vector* y = initialize_vector(size, 2, multiply);
    struct Vector* z = NULL;

    c = vector_add(a, b);      // returns a new vector
    z = vector_multiply(x, y); // returns a new vector

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
