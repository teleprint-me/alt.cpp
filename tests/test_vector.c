/**
 * @file tests/test_vector.c
 */

#include "../include/logger.h"
#include "../include/vector.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// keep the fixture and related tests as simple as reasonably possible
vector_t* vector_fixture(void) {
    size_t    dimensions = 2; // 2-dimensional vector, e.g. x, y
    vector_t* vector     = vector_create(dimensions);

    // we have to manually set x, y dimensions
    vector->elements[0] = 5; // set x (horizontal) axis
    vector->elements[1] = 3; // set y (vertical) axis

    return vector; // use vector_free(vector) to free the vector object
}

bool test_vector_create(void) {
    bool result = true;

    size_t    dimensions = 3; // 3-dimensional vector, e.g. x, y, z
    vector_t* vector     = vector_create(dimensions);

    if (NULL == vector) {
        // vector_create failed to allocate memory for vector
        result = false;
    } else if (NULL == vector->elements) {
        // vector_create failed to allocate memory for elements
        result = false;
    } else if (dimensions != vector->dimensions) {
        // vector_create failed to correctly set the vector dimensions
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "vector->dimensions(%zu) "
            "instead.\n",
            dimensions,
            vector->dimensions);
        result = false;
    }

    // Correctly destroy the vector and free its memory
    if (vector) {
        vector_free(vector);
    }

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_deep_copy(void) {
    bool   result     = true; // test result status
    size_t dimensions = 2;    // number of elements

    // Assuming vector_deep_copy takes a pointer to a float array and dimensions
    vector_t* original    = vector_create(dimensions);
    original->elements[0] = 1;
    original->elements[1] = 3;

    vector_t* deep_copy = vector_deep_copy(original);

    if (NULL == deep_copy) {
        // vector_deep_copy failed to allocate memory for vector
        result = false;
    } else if (deep_copy->elements[0] != 1 || deep_copy->elements[1] != 3) {
        // Elements do not match original vector's elements
        result = false;
    } else if (dimensions != deep_copy->dimensions) {
        // Failed to correctly set the vector dimensions
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "deep_copy->dimensions(%zu) "
            "instead.\n",
            dimensions,
            deep_copy->dimensions);
        result = false;
    }

    // Cleanup
    vector_free(original
    ); // Assuming this is how you'd free the original vector
    vector_free(deep_copy); // Free the copied vector

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_shallow_copy(void) {
    bool   result     = true;
    size_t dimensions = 2; // Example dimensions

    // Create an original vector and set some values
    vector_t* original    = vector_create(dimensions);
    original->elements[0] = 10;
    original->elements[1] = 20;

    // Perform a shallow copy
    vector_t* shallow_copy = vector_shallow_copy(original);

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
    } else if (dimensions != shallow_copy->dimensions) {
        // Check if both vectors share the same dimensions
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "shallow_copy->dimensions(%zu) "
            "instead.\n",
            dimensions,
            shallow_copy->dimensions);
    }

    // NOTE: Separate concerns for testing modifications
    // Modify the original vector and check if changes reflect in the shallow
    // copy
    original->elements[0] = 30; // Change the value
    if (shallow_copy->elements[0] != 30) {
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Changes in original vector not reflected in shallow copy.\n");
    }

    // Cleanup: Only destroy the original vector since shallow_copy shares the
    // same elements array
    vector_free(original);
    // Note: Depending on your implementation, you might need to free
    // shallow_copy itself, just not its elements array
    free(shallow_copy
    ); // Assuming shallow_copy is just a wrapper without its own elements array

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_free(void) {
    // TODO: Look into valgrind for more refined testing with memory
    // deallocation
    bool result = true;

    // Test destruction of a valid vector
    vector_t* vector = vector_create(2);
    if (NULL == vector) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to create a valid vector.\n");
    }

    vector_free(vector);

    // Test destruction with NULL vector
    // NOTE: vector_free logs an error when passing NULL as an argument.
    // if (vector_free(NULL)) {
    //     LOG(&global_logger, LOG_LEVEL_ERROR, "vector_free should return
    //     false for NULL input.\n"
    //     );
    //     result = false;
    // }

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

bool test_vector_magnitude(void) {
    bool  result    = true;
    float tolerance = 0.0001; // Tolerance for floating-point comparison

    // Create a 2-dimensional vector for a 3-4-5 triangle
    vector_t* vector = vector_create(2);
    if (NULL == vector) {
        result = false;
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to create vector.\n");
    } else {
        // Setting vector elements to (3, 4)
        vector->elements[0] = 3;
        vector->elements[1] = 4;

        // Calculate the magnitude
        float magnitude = vector_magnitude(vector);

        // The expected magnitude is 5.0 for a 3-4-5 Pythagorean triple
        float expected_magnitude = 5.0f;

        // Check if the calculated magnitude is within the tolerance of the
        // expected magnitude
        if (fabs(magnitude - expected_magnitude) > tolerance) {
            LOG(&global_logger,
                LOG_LEVEL_ERROR,
                "Magnitude calculation error: expected %f, got %f\n",
                expected_magnitude,
                magnitude);
            result = false;
        }

        // Clean up
        vector_free(vector);
    }

    printf("%s", result ? "." : "x");
    return result;
}

bool test_elementwise_operation(
    const char* operation_label,
    // Function pointer for expected result calculation
    vector_t* (*operation_elementwise)(const vector_t*, const vector_t*),
    float (*operation)(float a, float b)
) {
    vector_t* a = vector_create(3);
    vector_t* b = vector_create(3);
    vector_t* c = NULL;

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
                    "%s failed at index %zu with c->elements[%zu] = %f, "
                    "expected %f",
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

    vector_free(a);
    vector_free(b);

    if (c) { // guard against NULL and gracefully exit test
        vector_free(c);
    }

    printf("%s", result ? "." : "x");
    return result;
}

int main(void) {
    // NULL for const char* file_path
    // the log level can probably be set via a CLI param or config in the
    // future.
    initialize_global_logger(
        LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, "stream", stderr, NULL
    );

    bool result = true;

    result &= test_vector_create();
    result &= test_vector_deep_copy();
    result &= test_vector_shallow_copy();
    result &= test_vector_free();
    result &= test_elementwise_operation("add", vector_vector_add, scalar_add);
    result &= test_elementwise_operation(
        "subtract", vector_vector_subtract, scalar_subtract
    );
    result &= test_elementwise_operation(
        "multiply", vector_vector_multiply, scalar_multiply
    );
    result &= test_elementwise_operation(
        "divide", vector_vector_divide, scalar_divide
    );
    result &= test_vector_magnitude();

    if (result) {
        printf("All tests passed.\n");
    } else {
        printf("Tests failed. Please review the logs for more information.\n");
    }

    // logger_destroy(&global_logger); // destroy the logger
    // WARN: Freeing the global_logger causes a segmentation fault.
    // The global_logger is part of the call stack and is not allocated memory
    // at runtime. This is part of the LOG implementation details. Please refer
    // to docs for more info.

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
