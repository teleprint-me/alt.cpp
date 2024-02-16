/*
 * alt.cpp/source/vector.c
 *
 * Copyright © 2024 Austin Berrio
 *
 * Goal: Create a simple and easy to use Vector and Matrix API.
 *
 * Try sticking to using pure C. Only use math libs and C++ when absolutely necessary.
 *
 * NOTE: Prefixing related objects, functions, etc. assists with autocomplete.
 *
 */

#include "../include/vector.h"
#include "../include/logger.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vector lifecycle management
struct Vector* vector_create(size_t size) {
    struct Vector* vector = (struct Vector*) malloc(sizeof(struct Vector));
    if (NULL == vector) { // If no memory was allocated
        return NULL;      // Early return if vector creation failed
    }

    vector->elements = (float*) malloc(size * sizeof(float));
    if (NULL == vector->elements) { // Failed to allocate memory for elements
        free(vector);               // Free allocated vector memory to prevent leaks
        return NULL;                // Early return if vector creation failed
    }

    // After allocating vector->elements
    memset(vector->elements, 0, size * sizeof(float));

    vector->size = size; // track the size of the vector to prevent decay.

    return vector;
}

struct Vector* vector_deep_copy(const float elements[], size_t size) {
    struct Vector* deep_copy = vector_create(size);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (size_t i = 0; i < size; ++i) {
        deep_copy->elements[i] = elements[i];
    }

    return deep_copy;
}

struct Vector* vector_clone(const struct Vector* vector) {
    // note: this is just a convenience function for vector_deep_copy
    struct Vector* clone = vector_deep_copy(vector->elements, vector->size);
    if (NULL == clone) {
        return NULL;
    }

    return clone;
}

bool vector_destroy(struct Vector* vector) {
    if (NULL == vector) {
        return false;
    }

    if (NULL == vector->elements) {
        return false;
    }

    free(vector->elements);
    free(vector);

    return true;
}

// Vector mathematical operations
float vector_magnitude(const struct Vector* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (size_t i = 0; i < vector->size; i++) {
        sum += vector->elements[i] * vector->elements[i];
    }

    return sqrt(sum);
}

struct Vector* vector_normalize(struct Vector* vector, bool inplace) {
    float magnitude = vector_magnitude(vector);

    if (0 == magnitude) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Cannot normalize a zero-length vector.\n");
        return NULL;
    }

    if (inplace) {
        // scale the elements down by the magnitude to produce a unit vector
        for (size_t i = 0; i < vector->size; i++) {
            vector->elements[i] /= magnitude;
        }

        return vector;
    }

    struct Vector* unit = vector_create(vector->size);
    if (NULL == unit) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the normalized unit vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++) {
        unit->elements[i] = vector->elements[i] / magnitude;
    }

    return unit;
}

// Helper function for element-wise operations
float add(float x, float y) {
    return x + y;
}

float subtract(float x, float y) {
    return x - y;
}

float multiply(float x, float y) {
    return x * y;
}

float divide(float x, float y) {
    if (y == 0) {
        // Log a warning about division by zero to inform the user.
        LOG(&global_logger,
            LOG_LEVEL_WARN,
            "Attempted division by zero with x (%f) and y (%f). Returning default value 0.\n",
            x,
            y);
        // Return 0 as a default value to avoid division by zero while signaling the error.
        // This choice is documented and intentional to prevent `NaN` in vector calculations,
        // ensuring vectors remain valid for subsequent operations.
        return 0;
    }
    return x / y;
}

// Function to perform element-wise operation on two vectors
struct Vector* perform_elementwise_operation(
    const struct Vector* a, const struct Vector* b, float (*operation)(float, float)
) {
    if (a->size != b->size) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on vectors of size %zu and "
            "%zu.\n",
            a->size,
            b->size);
        return NULL;
    }

    struct Vector* c = vector_create(a->size);
    if (NULL == c) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to allocate memory for the resultant vector.\n"
        );
        return NULL;
    }

    // Perform element-wise operation
    for (size_t i = 0; i < a->size; i++) {
        c->elements[i] = operation(a->elements[i], b->elements[i]);
    }

    return c;
}

// Updated functions using the new helper function
struct Vector* vector_add(const struct Vector* a, const struct Vector* b) {
    return perform_elementwise_operation(a, b, add);
}

struct Vector* vector_subtract(const struct Vector* a, const struct Vector* b) {
    return perform_elementwise_operation(a, b, subtract);
}

struct Vector* vector_multiply(const struct Vector* a, const struct Vector* b) {
    return perform_elementwise_operation(a, b, multiply);
}

struct Vector* vector_divide(const struct Vector* a, const struct Vector* b) {
    return perform_elementwise_operation(a, b, divide);
}

float vector_dot_product(const struct Vector* a, const struct Vector* b) {
    float product = 0.0f;

    for (size_t i = 0; i < a->size; i++) {
        product += a->elements[i] * b->elements[i];
    }

    return product;
}

struct Vector* vector_cross_product(const struct Vector* a, const struct Vector* b) {}

// Coordinate transformation functions (prototypes to be defined)
struct Vector* polar_to_cartesian(const struct Vector* polar_vector) {}

struct Vector* cartesian_to_polar(const struct Vector* cartesian_vector) {}
