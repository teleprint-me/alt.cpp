/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/matrix.h
 *
 * @brief A simple and easy to use Matrix API
 *
 * This library provides a Matrix API designed for high performance
 * and ease of use in scientific computing, data analysis, and machine learning
 * applications. It leverages pure C for core operations to ensure maximum
 * compatibility and performance with a focus on simplicity, speed, and
 * flexibility.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef ALT_MATRIX_H
#define ALT_MATRIX_H

#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct Matrix {
    float* elements; ///< N-dimensional array representing the matrix elements.
    bool   is_transposed; ///< Indicates if the matrix is transposed.
    size_t rows;          ///< The number of rows (height) of the matrix.
    size_t columns;       ///< The number of columns (width) of the matrix.
} matrix_t;

// Matrix lifecycle management
matrix_t* matrix_create(const size_t rows, const size_t columns);
void      matrix_free(matrix_t* matrix);

// Element Access
float matrix_get_element(
    const matrix_t* matrix, const size_t row, const size_t column
);
void matrix_set_element(
    matrix_t* matrix, const size_t row, const size_t column, const float value
);

// Initialization Operations
void matrix_fill(matrix_t* matrix, const float value);
void matrix_random_fill(matrix_t* matrix, const float min, const float max);

// Copy Operations
matrix_t* matrix_deep_copy(const matrix_t* matrix);
matrix_t* matrix_shallow_copy(const matrix_t* matrix);

// Matrix Properties
bool matrix_is_zero(const matrix_t* matrix);
bool matrix_is_square(const matrix_t* matrix);
bool matrix_is_transposed(const matrix_t* matrix);
bool matrix_is_identity(const matrix_t* matrix);

// Matrix-Scalar Operations
matrix_t* matrix_scalar_operation(
    const matrix_t* matrix, float scalar, float (*operation)(float, float)
);
matrix_t* matrix_scalar_add(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_subtract(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_multiply(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_divide(const matrix_t* matrix, float scalar);

// Matrix-Vector Operations
matrix_t* matrix_vector_operation(
    const matrix_t* matrix,
    const vector_t* vector,
    float (*operation)(float, float)
);
matrix_t* matrix_vector_add(const matrix_t* matrix, const vector_t* vector);
matrix_t*
matrix_vector_subtract(const matrix_t* matrix, const vector_t* vector);
matrix_t*
matrix_vector_multiply(const matrix_t* matrix, const vector_t* vector);
matrix_t* matrix_vector_divide(const matrix_t* matrix, const vector_t* vector);

// Matrix-Matrix Operations
matrix_t* matrix_matrix_operation(
    const matrix_t* a, const matrix_t* b, float (*operation)(float, float)
);
matrix_t* matrix_matrix_add(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_subtract(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_multiply(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_divide(const matrix_t* a, const matrix_t* b);

// Matrix Transformations
matrix_t* matrix_transpose(matrix_t* matrix);
float     matrix_dot_product(const matrix_t* a, const matrix_t* b);

#endif // ALT_MATRIX_H
