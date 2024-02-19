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
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            size);
        return NULL; // Early return if vector creation failed
    }

    vector->elements = (float*) malloc(size * sizeof(float));
    if (NULL == vector->elements) { // Failed to allocate memory for elements
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to vector->elements.\n",
            size);
        free(vector); // Free allocated vector memory to prevent leaks
        return NULL;  // Early return if vector creation failed
    }

    // After allocating vector->elements
    memset(vector->elements, 0, size * sizeof(float));

    vector->size = size; // track the size of the vector to prevent decay.

    return vector;
}

struct Vector* vector_deep_copy(const struct Vector* vector) {
    struct Vector* deep_copy = vector_create(vector->size);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (size_t i = 0; i < vector->size; ++i) {
        deep_copy->elements[i] = vector->elements[i];
    }

    return deep_copy;
}

struct Vector* vector_shallow_copy(const struct Vector* vector) {
    if (NULL == vector) {
        return NULL;
    }

    // Allocate memory for the new Vector structure only, not for its elements
    struct Vector* new_vector = (struct Vector*) malloc(sizeof(struct Vector));
    if (NULL == new_vector) { // If no memory was allocated
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            sizeof(struct Vector));
        return NULL; // Early return if vector creation failed
    }

    // Copy all fields except elements (pointer to an array)
    new_vector->size = vector->size;

    // Assign the existing pointer to the new Vector structure
    new_vector->elements = vector->elements;

    return new_vector;
}

bool vector_destroy(struct Vector* vector) {
    if (NULL == vector) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Cannot free a NULL struct Vector.\n");
        return false;
    }

    if (NULL == vector->elements) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Cannot free a NULL vector->elements member.\n");
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

float vector_distance(const struct Vector* a, const struct Vector* b) {
    float distance_squared = 0.0f;

    if (a->size != b->size) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on vectors of size %zu and "
            "%zu.\n",
            a->size,
            b->size);
        return NAN;
    }

    for (size_t i = 0; i < a->size; ++i) {
        distance_squared += (a->elements[i] - b->elements[i]) * (a->elements[i] - b->elements[i]);
    }

    return sqrtf(distance_squared);
}

struct Vector* vector_scale(struct Vector* vector, float scalar, bool inplace) {
    if (vector == NULL) {
        return NULL;
    }

    if (inplace) { // block out-of-place vector scaling if in-place is true
        for (size_t i = 0; i < vector->size; ++i) {
            vector->elements[i] *= scalar; // scale the vector in-place
        }
        return vector; // return the scaled vector
    }

    // perform out-of-place vector scaling
    struct Vector* scaled_vector = vector_create(vector->size);
    if (scaled_vector == NULL) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to allocate memory for scaled vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->size; ++i) {
        scaled_vector->elements[i] = vector->elements[i] * scalar;
    }

    return scaled_vector;
}

float vector_mean(struct Vector* vector) {
    if (NULL == vector || 0 == vector->size) {
        return NAN; // Return NAN for invalid input
    }

    float sum = 0.0f;
    for (size_t i = 0; i < vector->size; i++) {
        if (isnan(vector->elements[i])) {
            // Log error and return NAN if any element is NaN
            LOG(&global_logger, LOG_LEVEL_ERROR, "NaN element found at index %zu.\n", i);
            return NAN;
        }
        sum += vector->elements[i];
    }

    return sum / vector->size; // Return the mean
}

struct Vector* vector_clip(struct Vector* vector, float min, float max, bool inplace) {
    if (NULL == vector || 0 == vector->size) {
        return NULL;
    }

    if (inplace) {
        for (size_t i = 0; i < vector->size; i++) {
            if (vector->elements[i] < min) {
                vector->elements[i] = min;
            }
            if (vector->elements[i] > max) {
                vector->elements[i] = max;
            }
        }

        return vector; // return as soon as possible
    }

    // create a vector if !inplace
    struct Vector* clipped_vector = vector_create(vector->size);
    if (NULL == clipped_vector) {
        return NULL; // NOTE: we can return and not log because vector_create logs the error for us
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->elements[i] < min) {
            clipped_vector->elements[i] = min;
        } else if (vector->elements[i] > max) {
            clipped_vector->elements[i] = max;
        } else {
            clipped_vector->elements[i] = vector->elements[i];
        }
    }

    // Return the newly created clipped vector
    return clipped_vector;
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
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Division by zero is undefined. Cannot divide x (%f) by y (%f).\n",
            x,
            y);
        return NAN; // Division by zero is undefined
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

// dot product is n-dimensional
float vector_dot_product(const struct Vector* a, const struct Vector* b) {
    if (a->size != b->size) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on vectors of size %zu and "
            "%zu.\n",
            a->size,
            b->size);
        return NAN;
    }

    float product = 0.0f;

    for (size_t i = 0; i < a->size; i++) {
        product += a->elements[i] * b->elements[i];
    }

    return product;
}

// cross product is 3-dimensional
struct Vector* vector_cross_product(const struct Vector* a, const struct Vector* b) {
    // Ensure both vectors are 3-dimensional.
    if (a->size != 3 || b->size != 3) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Cross product is only defined for 3-dimensional vectors.\n");
        return NULL;
    }

    struct Vector* result = vector_create(3);
    if (result == NULL) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Failed to allocate memory for cross product vector.\n"
        );
        return NULL;
    }

    // Calculate the components of the cross product vector.
    result->elements[0] = a->elements[1] * b->elements[2] - a->elements[2] * b->elements[1];
    result->elements[1] = a->elements[2] * b->elements[0] - a->elements[0] * b->elements[2];
    result->elements[2] = a->elements[0] * b->elements[1] - a->elements[1] * b->elements[0];

    return result;
}

// Coordinate transformation functions (prototypes to be defined)
struct Vector* vector_polar_to_cartesian(const struct Vector* polar_vector) {}

struct Vector* vector_cartesian_to_polar(const struct Vector* cartesian_vector) {}

// Function to calculate the softmax of a vector
struct Vector* vector_softmax(const struct Vector* vector) {
    // Calculate exponentials for each element in vector
    struct Vector* exponents = vector_create(vector->size);
    if (NULL == exponents) {
        return NULL;
    }

    float sum = 0.0;
    for (size_t i = 0; i < vector->size; i++) {
        exponents->elements[i]  = exp(vector->elements[i]);
        sum                    += exponents->elements[i];
    }

    // Divide each exponent by the total sum and store in output vector
    struct Vector* output = vector_create(vector->size);
    if (NULL == output) {
        vector_destroy(exponents);
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++) {
        output->elements[i] = exponents->elements[i] / sum;
    }

    // Clean up and return the result
    vector_destroy(exponents);
    return output;
}

float vector_cross_entropy(
    const struct Vector* prediction,
    const struct Vector* target,
    float                epsilon,
    float                normalization_factor
) {
    if (prediction == NULL || target == NULL) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "prediction and target cannot be NULL.\n");
        return NAN; // not a number
    }

    if (prediction->size != target->size) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. prediction->size(%zu) and "
            "target->size(%zu).\n",
            prediction->size,
            target->size);
    }

    float total_loss = 0.0f;

    for (size_t i = 0; i < prediction->size; ++i) {
        if (prediction->elements[i] <= 0.0f || target->elements[i] <= 0.0f
            || prediction->elements[i] > 1.0f || target->elements[i] > 1.0f) {
            LOG(&global_logger,
                LOG_LEVEL_ERROR,
                "Invalid input to cross entropy function: element at index %zu out of range.\n",
                i);
            return NAN; // not a number
        }

        float prediction_log  = logf(prediction->elements[i] + epsilon) * prediction->elements[i];
        float target_log      = logf(target->elements[i]) * target->elements[i];
        float elemental_loss  = prediction_log + target_log;
        total_loss           += elemental_loss;
    }

    // Normalize the loss by dividing it by the number of elements in the vectors
    float normalization_factor = powf(normalization_factor, (float) (prediction->size));
    return -total_loss / normalization_factor;
}
