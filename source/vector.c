/**
 * Copyright © 2024 Austin Berrio
 *
 * @file source/vector.c
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

/**
 * @brief lifecycle management
 */

/**
 * @brief Create a new N-dimensional vector
 *
 * This function dynamically allocates memory and initializes a new vector with
 * the specified number of dimensions. The values in the vector are set to zero
 * by default.
 *
 * @param dimensions Number of dimensions for the vector
 * @return A pointer to the newly created vector
 */
vector_t* vector_create(const size_t dimensions) {
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
    // NOTE: memset may be optimized away (under the as-if rules) if the object
    // modified by this function is not accessed again for the rest of its
    // lifetime (e.g., gcc bug 8537). For this reason, we do not employ its use
    // here.
    for (size_t i = 0; i < dimensions; i++) {
        vector->elements[i] = 0.0f;
    }

    vector->dimensions
        = dimensions; // track the dimensions of the vector to prevent decay.

    return vector;
}

/**
 * @brief Copy a given N-dimensional vector
 *
 * @note This function creates a deep copy of the input vector by allocating new
 * memory and duplicating its contents.
 *
 * @param vector Input vector
 * @return A pointer to the deep copied vector
 */
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

/**
 * @brief Create a shallow copy of an N-dimensional vector
 *
 * @note This function returns a pointer to the same allocated memory as the
 * input vector, effectively creating a reference (shallow) copy.
 *
 * @param vector Input vector
 * @return A pointer to the shallow copied vector
 */
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

/**
 * @brief Free an allocated N-dimensional vector
 *
 * This function deallocates memory associated with a given vector, releasing
 * any resources used during its creation.
 *
 * @note This function does not handle double freeing directly. To avoid
 * double freeing, ensure pointers are set to NULL after freeing.
 *
 * References:
 * - 7.22.3 Memory management functions on page 347
 * - <https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf>
 * - Rationale for setting the pointer to NULL:
 *   <https://stackoverflow.com/q/34284846/15147156>
 *
 * @param vector A pointer to the vector to be freed
 */
void vector_free(vector_t* vector) {
    if (vector == NULL) {
        return;
    }

    if (vector->elements) {
        free(vector->elements);
    }

    free(vector);
    // Note: Setting the pointer to NULL here would only affect the local copy
}

/**
 * @brief Element-wise operations
 *
 * These helper functions perform basic arithmetic on two floating-point values
 * and return the result.
 *
 * @param x First operand
 * @param y Second operand
 * @return Result of the operation
 */

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

/**
 * @brief Executor for element-wise vector-to-scalar functions
 *
 * This function applies a given operation to each corresponding pair of
 * elements in two vectors and returns the resulting vector.
 *
 * @param a First input vector
 * @param b Second input scalar
 * @param operation A pointer to the function performing the element-wise
 * operation
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_elementwise_operation(
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

/**
 * @brief Add a scalar value to an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to add
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_add(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_add);
}

/**
 * @brief Subtract a scalar value from an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to subtract
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_subtract(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_subtract);
}

/**
 * @brief Multiply a scalar value with an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to multiply
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_multiply(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_multiply);
}

/**
 * @brief Divide an N-dimensional vector by a scalar value
 *
 * @param a Input vector
 * @param b Scalar value to divide by
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_divide(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_divide);
}

/**
 * @brief Executor for element-wise vector-to-vector functions
 *
 * This function applies a given operation to each corresponding pair of
 * elements in two vectors and returns the resulting vector.
 *
 * @param a First input vector
 * @param b Second input vector
 * @param operation A pointer to the function performing the element-wise
 * operation
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_elementwise_operation(
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

/**
 * @brief Add two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_add(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_add);
}

/**
 * @brief Subtract one N-dimensional vector from another
 *
 * @param a First input vector
 * @param b Second input vector
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_subtract(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_subtract);
}

/**
 * @brief Multiply two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_multiply(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_multiply);
}

/**
 * @brief Divide an N-dimensional vector by another N-dimensional vector
 *
 * @param a First input vector
 * @param b Second input vector
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_divide(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_divide);
}

/**
 * @brief Common vector operations
 */

/**
 * @brief Determine the magnitude or length of an N-dimensional vector
 *
 * @param vector Input vector
 * @return The magnitude of the vector
 */
float vector_magnitude(const vector_t* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (size_t i = 0; i < vector->dimensions; i++) {
        sum += vector->elements[i] * vector->elements[i];
    }

    return sqrt(sum);
}

/**
 * @brief Calculate the distance between two given N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 * @return The distance between the two vectors
 */
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

/**
 * @brief Calculate the mean of an N-dimensional vector
 *
 * This function estimates the mean by weighted averaging.
 *
 * m(n) = [x(1) + x(2) + ... + x(n)] / n
 *
 * @param vector Input vector
 * @return The mean of the vector
 */
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

/**
 * @brief Low pass filter on an N-dimensional vector
 *
 * This function estimates the mean by low-pass filtering rather than averaging.
 * m(n + 1) = (1 - α) m(n) + α x(n + 1)
 *
 * @param vector Input vector
 * @param alpha Smoothing factor for the low-pass filter
 * @return The low-pass filtered mean of the vector
 *
 * References:
 * https://www.cs.princeton.edu/courses/archive/fall08/cos436/Duda/PR_learn/mean.htm
 *
 */
float vector_low_pass_filter(const vector_t* vector, float alpha) {
    LOG(&global_logger,
        LOG_LEVEL_ERROR,
        "Vector low pass filter is not currently implemented.\n");
    return -1.0f;
}

/**
 * @brief Normalize a given N-dimensional vector in place
 *
 * @param vector Input vector
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 * @return A pointer to the normalized vector
 */
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

/**
 * @brief Scale an N-dimensional vector by the specified factor
 *
 * @param vector Input vector
 * @param scalar Scaling factor
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 * @return A pointer to the scaled vector
 */
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

/**
 * @brief Clip an N-dimensional vector within a given range
 *
 * @param vector Input vector
 * @param min Minimum value for clipping
 * @param max Maximum value for clipping
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 * @return A pointer to the clipped vector
 */
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

/**
 * @brief Special vector operations
 */

/**
 * @brief Calculate the dot product of two N-dimensional vectors
 *
 * @note Dot product is n-dimensional
 *
 * @param a First input vector
 * @param b Second input vector
 * @return The dot product of the two vectors
 */
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

/**
 * @brief Return the cross product of two 3D vectors
 *
 * @note Cross product is 3-dimensional
 *
 * @param a First input vector (3D vector)
 * @param b Second input vector (3D vector)
 * @return A pointer to the cross product vector
 */
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

/**
 * @brief Special coordinates
 */

/**
 * @brief Convert polar coordinates to cartesian coordinates
 *
 * Polar coordinates are defined as the ordered pair (r, θ) names a point r
 * units from origin along the terminal side of angle θ in standard position
 * (origin to elements).
 *
 * @note x = r cos θ and y = r sin θ
 *
 * @param polar_vector Input vector in polar coordinates
 * @return A pointer to the vector in cartesian coordinates
 */
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

/**
 * @brief Convert cartesian coordinates to polar coordinates
 *
 * The derivation between polar and cartesian coordinates is to consider a point
 * P with the rectangular (x, y) and polar (r, θ) coordinates.
 *
 * @note r = ± √(x^2 + y^2) and tan θ = y / x
 *
 * @param cartesian_vector Input vector in cartesian coordinates
 * @return A pointer to the vector in polar coordinates
 */
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
