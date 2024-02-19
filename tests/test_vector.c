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
            LOG_LEVEL_ERROR,
            "Expected struct Vector to have size(%zu), got vector->size(%zu) instead.\n",
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
    bool   result = true; // test result status
    size_t size   = 2;    // number of elements

    // Assuming vector_deep_copy takes a pointer to a float array and size
    struct Vector* original = vector_create(size);
    original->elements[0]   = 1;
    original->elements[1]   = 3;

    struct Vector* deep_copy = vector_deep_copy(original);

    if (NULL == deep_copy) {
        // vector_deep_copy failed to allocate memory for vector
        result = false;
    } else if (deep_copy->elements[0] != 1 || deep_copy->elements[1] != 3) {
        // Elements do not match original vector's elements
        result = false;
    } else if (size != deep_copy->size) {
        // Failed to correctly set the vector size
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected struct Vector to have size(%zu), got deep_copy->size(%zu) "
            "instead.\n",
            size,
            deep_copy->size);
        result = false;
    }

    // Cleanup
    vector_destroy(original);  // Assuming this is how you'd free the original vector
    vector_destroy(deep_copy); // Free the copied vector

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_shallow_copy(void) {
    bool   result = true;
    size_t size   = 2; // Example size

    // Create an original vector and set some values
    struct Vector* original = vector_create(size);
    original->elements[0]   = 10;
    original->elements[1]   = 20;

    // Perform a shallow copy
    struct Vector* shallow_copy = vector_shallow_copy(original);

    // Test if the shallow copy was successful
    if (NULL == shallow_copy) {
        result = false;
        LOG(&global_logger, LOG_LEVEL_ERROR, "Shallow copy creation failed.\n");
    } else if (shallow_copy->elements != original->elements) {
        // Check if both vectors share the same elements array
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Elements array not shared between original and shallow copy.\n");
    } else if (size != shallow_copy->size) {
        // Check if both vectors share the same size
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected struct Vector to have size(%zu), got shallow_copy->size(%zu) "
            "instead.\n",
            size,
            shallow_copy->size);
    }

    // NOTE: Separate concerns for testing modifications
    // Modify the original vector and check if changes reflect in the shallow copy
    original->elements[0] = 30; // Change the value
    if (shallow_copy->elements[0] != 30) {
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Changes in original vector not reflected in shallow copy.\n");
    }

    // Cleanup: Only destroy the original vector since shallow_copy shares the same elements array
    vector_destroy(original);
    // Note: Depending on your implementation, you might need to free shallow_copy itself, just not
    // its elements array
    free(shallow_copy); // Assuming shallow_copy is just a wrapper without its own elements array

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_destroy(void) {
    // TODO: Look into valgrind for more refined testing with memory deallocation
    bool result = true;

    // Test destruction of a valid vector
    struct Vector* vector = vector_create(2);
    if (NULL == vector) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to create a valid vector.\n");
    }

    if (!vector_destroy(vector)) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to destroy a valid vector.\n");
        result = false;
    }

    // Test destruction with NULL vector
    // NOTE: vector_destroy logs an error when passing NULL as an argument.
    // if (vector_destroy(NULL)) {
    //     LOG(&global_logger, LOG_LEVEL_ERROR, "vector_destroy should return false for NULL
    //     input.\n"
    //     );
    //     result = false;
    // }

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_elementwise_operation(
    const char* operation_label,
    // Function pointer for expected result calculation
    struct Vector* (*operation_elementwise)(const struct Vector*, const struct Vector*),
    float (*operation)(float a, float b)
) {
    struct Vector* a = vector_create(3);
    struct Vector* b = vector_create(3);
    struct Vector* c = NULL;

    // Initialize vectors a and b
    for (size_t i = 0; i < 3; ++i) {
        a->elements[i] = 1.0f;
        b->elements[i] = 2.0f;
    }

    c           = operation_elementwise(a, b);
    bool result = true;
    if (NULL == c) {
        result = false;
    } else {
        for (size_t i = 0; i < 3; ++i) {
            float expected = operation(a->elements[i], b->elements[i]);
            if (c->elements[i] != expected) {
                LOG(&global_logger,
                    LOG_LEVEL_ERROR,
                    "%s failed at index %zu with c->elements[%zu] = %f, expected %f",
                    operation_label,
                    i,
                    i,
                    c->elements[i],
                    expected);
                result = false;
                break;
            }
        }
    }

    vector_destroy(a);
    vector_destroy(b);

    if (c) { // guard against NULL and gracefully exit test
        vector_destroy(c);
    }

    printf("%s", result ? "." : "x");
    return result;
}

int main(void) {
    // NULL for const char* file_path
    // the log level can probably be set via a CLI param or config in the future.
    initialize_global_logger(LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, "stream", stderr, NULL);

    bool result = true;

    result &= test_vector_create();
    result &= test_vector_deep_copy();
    result &= test_vector_shallow_copy();
    result &= test_vector_destroy();
    result &= test_elementwise_operation("add", vector_add, add);
    result &= test_elementwise_operation("subtract", vector_subtract, subtract);

    if (result) {
        printf("All tests passed.\n");
    } else {
        printf("Tests failed. Please review the logs for more information.\n");
    }

    // logger_destroy(&global_logger); // destroy the logger
    // WARN: Freeing the global_logger causes a segmentation fault.
    // The global_logger is part of the call stack and is not allocated memory at runtime.
    // This is part of the LOG implementation details. Please refer to docs for more info.

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
