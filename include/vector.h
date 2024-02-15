/*
 * alt.cpp/include/vector.h
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
#ifndef ALT_VECTOR
#define ALT_VECTOR

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

struct Vector* vector_create(size_t size);

struct Vector* vector_deep_copy(float elements[], size_t size);

// note: vector_clone is a convenience function and is substitutable with vector_deep_copy
struct Vector* vector_clone(struct Vector* vector);

bool vector_destroy(struct Vector* vector);

float vector_magnitude(struct Vector* vector);

struct Vector* vector_normalize(struct Vector* vector, bool inplace, struct Logger* logger);

struct Vector* vector_add(struct Vector* a, struct Vector* b, struct Logger* logger);

struct Vector* vector_subtract(struct Vector* a, struct Vector* b, struct Logger* logger);

float vector_dot_product(struct Vector* a, struct Vector* b);

struct Vector* vector_cross_product(struct Vector* a, struct Vector* b, struct Logger* logger);

// need to use math.h for polar to cartesian and vice-versa

#endif // ALT_VECTOR
