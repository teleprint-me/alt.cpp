/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_vector.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible. The
 * simpler, the better.
 */

#include "../include/logger.h"
#include "../include/vector.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/** Prototypes */

// Math utils
bool float_is_close(float a, float b, float relative, float absolute);

// Test fixtures
vector_t* vector_2d_fixture(float x, float y);
vector_t* vector_3d_fixture(float x, float y, float z);

// Vector lifecycle management
bool test_vector_create(void);
bool test_vector_deep_copy(void);
bool test_vector_shallow_copy(void);
bool test_vector_free(void);

// Element-wise operations
bool test_vector_vector_elementwise_operation(
    const char* operation_label,
    // Function pointer for expected result calculation
    vector_t* (*operation_elementwise)(const vector_t*, const vector_t*),
    float (*operation)(float a, float b)
);

// Common vector operations
bool test_vector_magnitude(void);
bool test_vector_distance(void);
bool test_vector_mean(void);
bool test_vector_normalize(void);

// Special vector operations
bool test_vector_dot_product(void);
bool test_vector_cross_product(void);

// Special vector coordinates
// TODO
// bool test_vector_polar_to_cartesian(void);
// bool test_vector_cartesian_to_polar(void);

/** Fixtures */

/**
 * @brief Determines if two floating-point values are approximately equal within
 * specified tolerances.
 *
 * This function checks whether the absolute difference between 'a' and 'b' is
 * less than or equal to the tolerance bounds, defined by both a relative
 * threshold (default: 1e-03) and an absolute threshold (default: 0.0).
 *
 * @param[in]   a       The first floating-point value.
 * @param[in]   b       The second floating-point value.
 * @param[in]   relative Relative tolerance for comparing values (default:
 * 1e-03).
 * @param[in]   absolute Absolute tolerance for comparing values (default: 0.0).
 *
 * @return true if the absolute difference between 'a' and 'b' is within the
 * tolerance bounds, false otherwise.
 */
bool float_is_close(
    float a, float b, float relative /*= 1e-3f*/, float absolute /*= 0.0f*/
) {
    return fabsf(a - b)
           <= fmaxf(relative * fmaxf(fabsf(a), fabsf(b)), absolute);
}

/**
 * @brief Creates a new vector object with specified x and y coordinates
 *
 * This fixture is used to create a new 2D vector object with the given
 * x (horizontal) and y (vertical) coordinates.
 *
 * @param[in] x The horizontal coordinate of the new vector
 * @param[in] y The vertical coordinate of the new vector
 *
 * @returns A pointer to the newly created vector object
 */
vector_t* vector_2d_fixture(float x, float y) {
    const size_t dimensions = 2; // 2-dimensional vector
    vector_t*    vector     = vector_create(dimensions);

    // set elements with provided coordinates
    vector->elements[0] = x; // horizontal axis representing the width
    vector->elements[1] = y; // vertical axis representing the height

    return vector; // use vector_free(vector) to free the vector object
}

/**
 * @brief Creates a new vector object with specified x, y and z coordinates
 *
 * This fixture is used to create a new 3D vector object with the given
 * x (horizontal), y (vertical) and z (depth) coordinates.
 *
 * @param[in] x The horizontal coordinate of the new vector
 * @param[in] y The vertical coordinate of the new vector
 * @param[in] z The depth coordinate of the new vector
 *
 * @returns A pointer to the newly created vector object
 */
vector_t* vector_3d_fixture(float x, float y, float z) {
    const size_t dimensions = 3; // 3-dimensional vector
    vector_t*    vector     = vector_create(dimensions);

    // set elements with provided coordinates
    vector->elements[0] = x; // horizontal axis representing the width
    vector->elements[1] = y; // vertical axis representing the height
    vector->elements[2] = z; // orthogonal axis representing the depth

    return vector; // use vector_free(vector) to free the vector object
}

/** Unit Tests */

/**
 * @brief Test the correctness of vector creation
 *
 * This function tests that the `vector_create()` implementation
 * correctly allocates memory and initializes a new N-dimensional
 * vector with zeros.
 */
bool test_vector_create(void) {
    bool result = true;

    // Test with a valid number of dimensions
    const size_t dimensions = 3;
    vector_t*    vector     = vector_create(dimensions);

    if (NULL == vector) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for a %zu-dimensional vector.\n",
            dimensions);
        result = false;
    } else if (NULL == vector->elements) {
        // vector_create failed to allocate memory for elements
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to vector->elements.\n",
            dimensions * sizeof(float));
        result = false;
    } else if (dimensions != vector->dimensions) {
        // vector_create failed to correctly set the vector dimensions
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "vector->dimensions(%zu)"
            " instead.\n",
            dimensions,
            vector->dimensions);
        result = false;
    } else {
        // Check if the elements are initialized with zeros
        for (size_t i = 0; i < dimensions; ++i) {
            assert(vector->elements[i] == 0.0f);
        }

        // Correctly destroy the vector and free its memory
        if (vector) {
            vector_free(vector);
        }
    }

    printf("%s", result ? "." : "x");
    return result;
}

/**
 * @brief Test if the vector_deep_copy implementation correctly duplicates a
 * given N-dimensional input vector by allocating new memory for the copied
 * data.
 *
 * This test verifies that `vector_deep_copy()` function works as expected under
 * various conditions, including:
 * - Correctly allocates new memory and copies elements from the original vector
 * - Maintains the same number of dimensions as the original vector
 *
 * @returns true if all tests pass; false otherwise
 */
bool test_vector_deep_copy(void) {
    bool result = true; // test result status

    // original->elements[0] = 1
    // original->elements[1] = 3
    // original->dimensions  = 2
    vector_t* original = vector_2d_fixture(1, 3);

    // vector_deep_copy takes a pointer to a vector_t object
    vector_t* deep_copy = vector_deep_copy(original);

    if (NULL == deep_copy) {
        // vector_deep_copy failed to allocate memory for vector
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for deep copy of the vector.\n");
        result = false;
    } else if (deep_copy->elements[0] != 1 || deep_copy->elements[1] != 3) {
        // Elements do not match original vector's elements
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Deep copy elements do not match original vector's elements.\n");
        result = false;
    } else if (original->dimensions != deep_copy->dimensions) {
        // Failed to correctly set the vector dimensions
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "deep_copy->dimensions(%zu) instead.\n",
            original->dimensions,
            deep_copy->dimensions);
        result = false;
    } else {
        // Ensure deep copy is indeed a separate memory allocation
        original->elements[0] = 2; // Modify original vector
        if (deep_copy->elements[0] == 2) {
            LOG(&global_logger,
                LOG_LEVEL_ERROR,
                "Deep copy shares memory with original vector. "
                "Elements should be independent.\n");
            result = false;
        }
    }

    // Cleanup
    vector_free(original);  // Free the original vector
    vector_free(deep_copy); // Free the copied vector

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

/**
 * @brief Test if shallow copying an N-dimensional vector creates a reference
 * (shallow) copy with shared memory.
 *
 * This test verifies that `vector_shallow_copy()` works as expected under
 * various conditions, including:
 * - Shallow copies are created successfully
 * - The original and shallow copied vectors share the same elements array
 * - Both vectors have the same number of dimensions
 * - Modifications to either vector affect both since they share memory
 */
bool test_vector_shallow_copy(void) {
    bool result = true;

    // Create an original vector and set some values
    vector_t* original = vector_2d_fixture(10, 20);

    // Perform a shallow copy
    vector_t* shallow_copy = vector_shallow_copy(original);

    // Check if both vectors share the same elements array
    if (original->elements != shallow_copy->elements) {
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Elements array not shared between original and shallow copy.\n");
    } else if (original->dimensions != shallow_copy->dimensions) {
        // Check if both vectors share the same dimensions
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Expected vector_t to have dimensions(%zu), got "
            "shallow_copy->dimensions(%zu) instead.\n",
            original->dimensions,
            shallow_copy->dimensions);
    }

    // Modify the original vector and check if changes reflect in the shallow
    // copy
    original->elements[0] = 30; // Change the value
    if (shallow_copy->elements[0] != 30) {
        result = false;
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Changes to the original vector not reflected in the shallow "
            "copy.\n");
    }

    // Cleanup: Only destroy the original vector since shallow_copy shares the
    // same elements array
    vector_free(original);
    // Free the shallow_copy itself, assuming shallow_copy is just a wrapper
    // without its own elements array
    if (shallow_copy) {
        free(shallow_copy);
        // note: we cannot use vector_free here.
        // using vector_free will result in a double free incident.
    }

    printf("%s", result ? "." : "x");
    return result; // Return the actual result of the test
}

/**
 * @brief Test if vector deallocation is performed correctly
 *
 * This test verifies that the vector_free implementation works as expected,
 * ensuring proper memory deallocation of both valid and invalid vectors.
 *
 * @note This function does not handle double freeing directly. To avoid
 * double freeing, ensure pointers are set to NULL after freeing.
 *
 * References:
 * - 7.22.3 Memory management functions on page 347
 * - <https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf>
 * - Rationale for setting the pointer to NULL:
 *   <https://stackoverflow.com/q/34284846/15147156>
 */
bool test_vector_free(void) {
    bool result = true;

    // Create a vector using the fixture
    vector_t* vector = vector_2d_fixture(10, 20);

    // Test destruction of a valid vector
    if (NULL == vector) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to create a valid vector.\n");
        result = false;
    }

    // Free the vector
    vector_free(vector);
    vector = NULL; // Comment this out if you want to explicitly test
                   // double-free handling.

    // Ensure the vector is properly destroyed and pointer is set to NULL
    if (NULL != vector) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to set vector pointer to NULL after free.\n");
        result = false;
    }

    // Test destruction with NULL vector
    vector_free(NULL);   // This should not cause any errors or logging
    vector_free(vector); // This should handle already NULL pointers gracefully

    printf("%s", result ? "." : "x");

    return result;
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

bool test_vector_distance(void) {
    bool result = false; // set to true once implemented
    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_mean(void) {
    bool result = false; // set to true once implemented
    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_normalize(void) {
    bool result = false; // set to true once implemented
    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_dot_product(void) {
    bool result = false; // set to true once implemented
    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_cross_product(void) {
    bool result = false; // set to true once implemented
    printf("%s", result ? "." : "x");
    return result;
}

bool test_vector_vector_elementwise_operation(
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

    // Vector lifecycle management
    result &= test_vector_create();
    result &= test_vector_deep_copy();
    result &= test_vector_shallow_copy();
    result &= test_vector_free();

    // Element-wise operations

    // test vector scalar elementwise operations
    // TODO

    // test vector vector elementwise operations
    result &= test_vector_vector_elementwise_operation(
        "add", vector_vector_add, scalar_add
    );
    result &= test_vector_vector_elementwise_operation(
        "subtract", vector_vector_subtract, scalar_subtract
    );
    result &= test_vector_vector_elementwise_operation(
        "multiply", vector_vector_multiply, scalar_multiply
    );
    result &= test_vector_vector_elementwise_operation(
        "divide", vector_vector_divide, scalar_divide
    );

    // Common vector operations

    result &= test_vector_magnitude();
    result &= test_vector_distance();
    result &= test_vector_mean();
    result &= test_vector_normalize();

    // Special vector operations
    result &= test_vector_dot_product();
    result &= test_vector_cross_product();

    printf("\n");
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
