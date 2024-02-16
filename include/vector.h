/*
 * alt.cpp/include/vector.h
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
#ifndef ALT_VECTOR_H
#define ALT_VECTOR_H

#include "logger.h" // Include the logger header for error logging functionality

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vector {
    float* elements; // n-dimensional vector space
    size_t size;     // number of elements in vector space
};

// Vector lifecycle management
struct Vector* vector_create(size_t size);
struct Vector* vector_deep_copy(const float elements[], size_t size);
struct Vector* vector_clone(const struct Vector* vector);
bool           vector_destroy(struct Vector* vector);

// Vector mathematical operations
float          vector_magnitude(const struct Vector* vector);
struct Vector* vector_normalize(struct Vector* vector, bool inplace, struct Logger* logger);

// Helper functions for element-wise operations
float add(float x, float y);
float subtract(float x, float y);
float multiply(float x, float y);
float divide(float x, float y);

// Element wise operations
struct Vector* perform_elementwise_operation(
    const struct Vector* a,
    const struct Vector* b,
    struct Logger*       logger,
    float (*operation)(float, float) // pass function by value
);

struct Vector* vector_add(const struct Vector* a, const struct Vector* b, struct Logger* logger);
struct Vector*
vector_subtract(const struct Vector* a, const struct Vector* b, struct Logger* logger);
struct Vector*
vector_multiply(const struct Vector* a, const struct Vector* b, struct Logger* logger);
struct Vector* vector_divide(const struct Vector* a, const struct Vector* b, struct Logger* logger);

// Calculate vector products
float vector_dot_product(const struct Vector* a, const struct Vector* b);
struct Vector*
vector_cross_product(const struct Vector* a, const struct Vector* b, struct Logger* logger);

// Coordinate transformation functions (prototypes to be defined)
struct Vector* polar_to_cartesian(const struct Vector* polar_vector);
struct Vector* cartesian_to_polar(const struct Vector* cartesian_vector);

#endif // ALT_VECTOR_H
