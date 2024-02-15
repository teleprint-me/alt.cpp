/*
 * alt.cpp/source/vector.c
 *
 * Copyright © 2024 Austin Berrio
 *
 * Goal: Create a simple and easy to use Vector and Matrix API.
 *
 * Try to stick to using pure C. Only use math libs and C++ when absolutely necessary.
 *
 * NOTES:
 * technically, we just have scalars, vectors, and matrices
 * so we only have a number (typically a float); a point on a line?
 * list of numbers; the standard vector, e.g. [i_1, i_2, i_3, ..., i_n]
 * a "grid" of numbers; the standard matrix, e.g. [[n_1, n_2], [n_3, n_4]]
 * the tricky part is managing the coordinates and dimensions
 *
 * NOTE: Prefixing related objects, functions, etc. assists with autocomplete.
 *
 */

#include "../include/log.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vector {
    float* elements; // n-dimensional vector space
    size_t size;     // number of elements in vector space
    // stride is probably more important when dealing with matrices rather than vectors
};

struct Vector* vector_create(size_t size) {
    struct Vector* vector = (struct Vector*)malloc(sizeof(struct Vector));
    if (NULL == vector) { // If no memory was allocated
        return NULL;      // Early return if vector creation failed
    }

    vector->elements = (float*)malloc(size * sizeof(float));
    if (NULL == vector->elements) { // Failed to allocate memory for elements
        free(vector);               // Free allocated vector memory to prevent leaks
        return NULL;                // Early return if vector creation failed
    }

    // After allocating vector->elements
    memset(vector->elements, 0, size * sizeof(float));

    vector->size = size; // track the size of the vector to prevent decay.

    return vector;
}

struct Vector* vector_deep_copy(float elements[], size_t size) {
    struct Vector* deep_copy = vector_create(size);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (size_t i = 0; i < size; ++i) {
        deep_copy->elements[i] = elements[i];
    }

    return deep_copy;
}

struct Vector* vector_clone(struct Vector* vector) {
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

float vector_magnitude(struct Vector* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (size_t i = 0; i < vector->size; i++) {
        sum += vector->elements[i] * vector->elements[i];
    }

    return sqrt(sum);
}

struct Vector* vector_normalize(struct Vector* vector, bool inplace, struct Logger* logger) {
    struct Vector* normalized = NULL;
    float          magnitude  = vector_magnitude(vector);

    if (0 == magnitude) {
        LOG(logger, LOG_LEVEL_ERROR, "Cannot normalize a zero-length vector.\n");
        return NULL;
    }

    if (inplace) {
        // scale the elements down by the magnitude to produce a unit vector
        for (size_t i = 0; i < vector->size; i++) {
            vector->elements[i] /= magnitude;
        }

        return vector;
    }

    normalized = vector_create(vector->size);
    if (NULL == normalized) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to allocate memory for the normalized vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++) {
        normalized->elements[i] = vector->elements[i] / magnitude;
    }

    return normalized;
}

struct Vector* vector_add(struct Vector* a, struct Vector* b, struct Logger* logger) {
    if (a->size != b->size) {
        LOG(logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot add vectors of size %zu and %zu.\n",
            a->size,
            b->size);
        return NULL;
    }

    struct Vector* c = vector_create(a->size);
    if (NULL == c) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // element-wise addition
    for (size_t i = 0; i < a->size; i++) {
        c->elements[i] = a->elements[i] + b->elements[i];
    }

    return c;
}

struct Vector* vector_subtract(struct Vector* a, struct Vector* b, struct Logger* logger) {
    if (a->size != b->size) {
        LOG(logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot subtract vectors of size %zu and %zu.\n",
            a->size,
            b->size);
        return NULL;
    }

    struct Vector* c = vector_create(a->size);
    if (NULL == c) {
        LOG(logger, LOG_LEVEL_ERROR, "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // element-wise subtraction
    for (size_t i = 0; i < a->size; i++) {
        c->elements[i] = a->elements[i] - b->elements[i];
    }

    return c;
}

float vector_dot_product(struct Vector* a, struct Vector* b) {}

struct Vector* vector_cross_product(struct Vector* a, struct Vector* b, struct Logger* logger) {}

// need to use math.h for polar to cartesian and vice-versa
