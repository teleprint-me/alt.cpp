/*
 * alt.cpp/tests/test_vector.c
 */

#include "../include/logger.h"
#include "../include/vector.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// keep the fixture and related tests as simple as reasonably possible
struct Vector* vector_fixture(void) {
    size_t         size   = 2; // 2-dimensional vector, e.g. x, y
    struct Vector* vector = vector_create(size);

    // we have to manually set x, y dimensions
    vector->elements[0] = 5; // set x (horizontal) axis
    vector->elements[1] = 3; // set y (vertical) axis

    return vector; // use vector_destroy(vector) to free the vector object
}

bool test_vector_create(void) {
    bool result = true;

    size_t         size   = 3; // 3-dimensional vector, e.g. x, y, z
    struct Vector* vector = vector_create(size);

    if (NULL == vector) {
        // vector_create failed to allocate memory for vector
        result = false;
    } else if (NULL == vector->elements) {
        // vector_create failed to allocate memory for elements
        result = false;
    } else if (size != vector->size) {
        // vector_create failed to correctly set the vector size
        LOG(&global_logger,
            LOG_LEVEL_DEBUG,
            "Failed to set vector size: expected %zu, got %zu instead.",
            size,
            vector->size);
        result = false;
    }

    // Correctly destroy the vector and free its memory
    if (vector) {
        vector_destroy(vector);
    }

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_deep_copy(void) {
    bool result = true;

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_clone(void) {
    bool result = true;

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_destroy(void) {
    bool result = true;

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_addition(void) {
    struct Vector* a = vector_create(3);
    struct Vector* b = vector_create(3);
    struct Vector* c = NULL;

    // Set elements for a and b
    for (size_t i = 0; i < 3; ++i) {
        a->elements[i] = 1.0f; // Assuming your vector elements are of type float
        b->elements[i] = 2.0f;
    }

    // Perform addition
    c = vector_add(a, b);

    // Assert the results
    bool result = true;
    for (size_t i = 0; i < 3; ++i) {
        if (c->elements[i] != 3.0f) {
            result = false;
            break;
        }
    }

    vector_destroy(a);
    vector_destroy(b);
    vector_destroy(c);

    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_subtract(void) {
    struct Vector* a = vector_create(3);
    struct Vector* b = vector_create(3);
    struct Vector* c = NULL;

    a->elements[0] = memset(a->elements, 1, 3 * sizeof(float));
    memset(b->elements, 2, 3 * sizeof(float));

    c = vector_subtract(a, b); // creates a new vector dynamically

    // test elements results

    vector_destroy(a);
    vector_destroy(b);
    vector_destroy(c);
}

int main(void) {
    // NULL for const char* file_path
    initialize_global_logger(LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, "stream", stderr, NULL);

    bool result = true;

    result &= test_vector_create();
    result &= test_vector_addition();
    result &= test_vector_subtraction();

    if (result) {
        printf("All tests passed.\n");
    } else {
        printf("Tests failed. Please review the logs for more information.\n");
    }

    LOG_DESTROY(); // destroy the logger

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
