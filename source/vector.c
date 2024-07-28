/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vector.h
 *
 * @brief A simple and easy-to-use Vector API in C
 *
 * This header file provides an implementation for basic mathematical operations
 * on N-dimensional vectors, including creation, destruction, element-wise
 * functions, scalar-based functions, vector-based functions, common vector
 * operations, special vector operations, and conversion between polar and
 * cartesian coordinates.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "../include/vector.h"
#include "../include/logger.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vector lifecycle management
vector_t* vector_create(size_t dimensions) {
    vector_t* vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == vector) { // If no memory was allocated
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            dimensions);
        return NULL; // Early return if vector creation failed
    }

    vector->elements = (float*) malloc(dimensions * sizeof(float));
    if (NULL == vector->elements) { // Failed to allocate memory for elements
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to vector->elements.\n",
            dimensions);
        free(vector); // Free allocated vector memory to prevent leaks
        return NULL;  // Early return if vector creation failed
    }

    // After allocating vector->elements
    memset(vector->elements, 0, dimensions * sizeof(float));

    vector->dimensions
        = dimensions; // track the dimensions of the vector to prevent decay.

    return vector;
}

void vector_free(vector_t* vector) {
    if (vector->elements) {
        free(vector->elements);
    }

    if (vector) {
        free(vector);
    }
}

vector_t* vector_deep_copy(const vector_t* vector) {
    vector_t* deep_copy = vector_create(vector->dimensions);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (size_t i = 0; i < vector->dimensions; ++i) {
        deep_copy->elements[i] = vector->elements[i];
    }

    return deep_copy;
}

vector_t* vector_shallow_copy(const vector_t* vector) {
    if (NULL == vector) {
        return NULL;
    }

    // Allocate memory for the new Vector structure only, not for its elements
    vector_t* new_vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == new_vector) { // If no memory was allocated
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            sizeof(vector_t));
        return NULL; // Early return if vector creation failed
    }

    // Copy all fields except elements (pointer to an array)
    new_vector->dimensions = vector->dimensions;

    // Assign the existing pointer to the new Vector structure
    new_vector->elements = vector->elements;

    return new_vector;
}

// Vector mathematical operations
float vector_magnitude(const vector_t* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (size_t i = 0; i < vector->dimensions; i++) {
        sum += vector->elements[i] * vector->elements[i];
    }

    return sqrt(sum);
}

vector_t* vector_normalize(vector_t* vector, bool inplace) {
    float magnitude = vector_magnitude(vector);

    if (0 == magnitude) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Cannot normalize a zero-length vector.\n");
        return NULL;
    }

    if (inplace) {
        // scale the elements down by the magnitude to produce a unit vector
        for (size_t i = 0; i < vector->dimensions; i++) {
            vector->elements[i] /= magnitude;
        }

        return vector;
    }

    vector_t* unit = vector_create(vector->dimensions);
    if (NULL == unit) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the normalized unit vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->dimensions; i++) {
        unit->elements[i] = vector->elements[i] / magnitude;
    }

    return unit;
}

float vector_distance(const vector_t* a, const vector_t* b) {
    float distance_squared = 0.0f;

    if (a->dimensions != b->dimensions) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->dimensions,
            b->dimensions);
        return NAN;
    }

    for (size_t i = 0; i < a->dimensions; ++i) {
        distance_squared += (a->elements[i] - b->elements[i])
                            * (a->elements[i] - b->elements[i]);
    }

    return sqrtf(distance_squared);
}

vector_t* vector_scale(vector_t* vector, float scalar, bool inplace) {
    if (vector == NULL) {
        return NULL;
    }

    if (inplace) { // block out-of-place vector scaling if in-place is true
        for (size_t i = 0; i < vector->dimensions; ++i) {
            vector->elements[i] *= scalar; // scale the vector in-place
        }
        return vector; // return the scaled vector
    }

    // perform out-of-place vector scaling
    vector_t* scaled_vector = vector_create(vector->dimensions);
    if (scaled_vector == NULL) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for scaled vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->dimensions; ++i) {
        scaled_vector->elements[i] = vector->elements[i] * scalar;
    }

    return scaled_vector;
}

float vector_mean(const vector_t* vector) {
    if (NULL == vector || 0 == vector->dimensions) {
        return NAN; // Return NAN for invalid input
    }

    float sum = 0.0f;
    for (size_t i = 0; i < vector->dimensions; i++) {
        if (isnan(vector->elements[i])) {
            // Log error and return NAN if any element is NaN
            LOG(&global_logger,
                LOG_LEVEL_ERROR,
                "NaN element found at index %zu.\n",
                i);
            return NAN;
        }
        sum += vector->elements[i];
    }

    return sum / vector->dimensions; // Return the mean
}

vector_t* vector_clip(vector_t* vector, float min, float max, bool inplace) {
    if (NULL == vector || 0 == vector->dimensions) {
        return NULL;
    }

    if (inplace) {
        for (size_t i = 0; i < vector->dimensions; i++) {
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
    vector_t* clipped_vector = vector_create(vector->dimensions);
    if (NULL == clipped_vector) {
        return NULL; // NOTE: we can return and not log because vector_create
                     // logs the error for us
    }

    for (size_t i = 0; i < vector->dimensions; i++) {
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
float scalar_add(float x, float y) {
    return x + y;
}

float scalar_subtract(float x, float y) {
    return x - y;
}

float scalar_multiply(float x, float y) {
    return x * y;
}

float scalar_divide(float x, float y) {
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

// Function to perform element-wise operation between a vector and scalar value
vector_t* scalar_elementwise_operation(
    const vector_t* a, const float b, float (*operation)(float, float)
) {
    vector_t* c = vector_create(a->dimensions);
    if (NULL == c) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (size_t i = 0; i < a->dimensions; i++) {
        c->elements[i] = operation(a->elements[i], b);
    }

    return c;
}

vector_t* vector_scalar_add(const vector_t* a, const float b) {
    return scalar_elementwise_operation(a, b, scalar_add);
}

vector_t* vector_scalar_subtract(const vector_t* a, const float b) {
    return scalar_elementwise_operation(a, b, scalar_subtract);
}

vector_t* vector_scalar_multiply(const vector_t* a, const float b) {
    return scalar_elementwise_operation(a, b, scalar_multiply);
}

vector_t* vector_scalar_divide(const vector_t* a, const float b) {
    return scalar_elementwise_operation(a, b, scalar_divide);
}

// Function to perform element-wise operation on two vectors
vector_t* vector_elementwise_operation(
    const vector_t* a, const vector_t* b, float (*operation)(float, float)
) {
    if (a->dimensions != b->dimensions) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->dimensions,
            b->dimensions);
        return NULL;
    }

    vector_t* c = vector_create(a->dimensions);
    if (NULL == c) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (size_t i = 0; i < a->dimensions; i++) {
        c->elements[i] = operation(a->elements[i], b->elements[i]);
    }

    return c;
}

// Updated functions using the new helper function
vector_t* vector_vector_add(const vector_t* a, const vector_t* b) {
    return vector_elementwise_operation(a, b, scalar_add);
}

vector_t* vector_vector_subtract(const vector_t* a, const vector_t* b) {
    return vector_elementwise_operation(a, b, scalar_subtract);
}

vector_t* vector_vector_multiply(const vector_t* a, const vector_t* b) {
    return vector_elementwise_operation(a, b, scalar_multiply);
}

vector_t* vector_vector_divide(const vector_t* a, const vector_t* b) {
    return vector_elementwise_operation(a, b, scalar_divide);
}

// dot product is n-dimensional
float vector_dot_product(const vector_t* a, const vector_t* b) {
    if (a->dimensions != b->dimensions) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->dimensions,
            b->dimensions);
        return NAN;
    }

    float product = 0.0f;

    for (size_t i = 0; i < a->dimensions; i++) {
        product += a->elements[i] * b->elements[i];
    }

    return product;
}

// cross product is 3-dimensional
vector_t* vector_cross_product(const vector_t* a, const vector_t* b) {
    // Ensure both vectors are 3-dimensional.
    if (a->dimensions != 3 || b->dimensions != 3) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Cross product is only defined for 3-dimensional vectors.\n");
        return NULL;
    }

    vector_t* result = vector_create(3);
    if (result == NULL) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for cross product vector.\n");
        return NULL;
    }

    // Calculate the components of the cross product vector.
    result->elements[0]
        = a->elements[1] * b->elements[2] - a->elements[2] * b->elements[1];
    result->elements[1]
        = a->elements[2] * b->elements[0] - a->elements[0] * b->elements[2];
    result->elements[2]
        = a->elements[0] * b->elements[1] - a->elements[1] * b->elements[0];

    return result;
}

// Formula is as defined:
// r = ± √(x^2 + y^2)
// tan θ = y / x
// x = r cos θ
// y = r sin θ
// theta is the angle between the hypotenuse and the x-axis.
// P is (x, y) is (r, θ)
// x is the base, y is the height, assuming a right angle between rise over run
// r is hypotenuse

// Coordinate transformation functions

// Polar coordinates are defined as the ordered pair (r, θ) names a point r
// units from origin along the terminal side of angle θ in standard position
// (origin to elements).

// x = r cos θ and y = r sin θ
vector_t* vector_polar_to_cartesian(const vector_t* polar_vector) {
    if (NULL == polar_vector || polar_vector->dimensions != 2) {
        return NULL; // Return NULL if input is invalid
    }

    vector_t* cartesian_vector = vector_create(2);
    if (NULL == cartesian_vector) {
        return NULL; // Return NULL if memory allocation fails
    }

    // radii/radius/ray all seem equivalently apropos
    // perhaps ray is best suited?
    float r     = polar_vector->elements[0];
    float theta = polar_vector->elements[1];

    cartesian_vector->elements[0] = r * cosf(theta); // x = r * cos(θ)
    cartesian_vector->elements[1] = r * sinf(theta); // y = r * sin(θ)

    return cartesian_vector;
}

// The derivation between polar and cartesian coordinates is to consider a point
// P with the rectangular (x, y) and polar (r, θ) coordinates.

// r = ± √(x^2 + y^2) and tan θ = y / x
vector_t* vector_cartesian_to_polar(const vector_t* cartesian_vector) {
    if (NULL == cartesian_vector || cartesian_vector->dimensions != 2) {
        return NULL; // Return NULL if input is invalid
    }

    vector_t* polar_vector = vector_create(2);
    if (NULL == polar_vector) {
        return NULL; // Return NULL if memory allocation fails
    }

    float x = cartesian_vector->elements[0];
    float y = cartesian_vector->elements[1];

    polar_vector->elements[0] = sqrtf(x * x + y * y); // r = √(x^2 + y^2)
    polar_vector->elements[1] = atan2f(y, x);         // θ = atan (y, x)

    return polar_vector;
}
