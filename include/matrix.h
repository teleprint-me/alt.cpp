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

//
typedef struct Matrix {
    float* elements; ///< N-dimensional array representing the matrix elements.

    bool is_transposed; ///< A boolean value indicating if the matrix is
                        ///< transposed

    size_t rows;    ///< The number of rows (height) of the matrix.
    size_t columns; ///< The number of columns (width) of the matrix.
} matrix_t;

//
matrix_t* matrix_create(size_t rows, size_t columns);
void      matrix_free(matrix_t* matrix);

//
float get_element(matrix_t* matrix, size_t row, size_t column);
void  set_element(matrix_t* matrix, size_t row, size_t column, float value);

//
matrix_t* matrix_deep_copy(const matrix_t* matrix);
matrix_t* matrix_shallow_copy(const matrix_t* matrix);

//
void matrix_fill(matrix_t* matrix, float value);
void matrix_random(matrix_t* matrix, float min, float max);

//
bool matrix_is_zero(matrix_t* matrix);
bool matrix_is_square(matrix_t* matrix);
bool matrix_is_transposed(matrix_t* matrix);
bool matrix_is_identity(matrix_t* matrix);

// matrix-scalar
matrix_t* matrix_scalar_elementwise_operation(
    const matrix_t* a, const float b, float (*operation)(float, float)
);
matrix_t* matrix_scalar_add(const matrix_t* a, const float b);
matrix_t* matrix_scalar_subtract(const matrix_t* a, const float b);
matrix_t* matrix_scalar_multiply(const matrix_t* a, const float b);
matrix_t* matrix_scalar_divide(const matrix_t* a, const float b);

// matrix-vector
matrix_t* matrix_vector_elementwise_operation(
    const matrix_t* a, const vector_t* b, float (*operation)(float, float)
);
matrix_t* matrix_vector_add(const matrix_t* a, const vector_t* b);
matrix_t* matrix_vector_subtract(const matrix_t* a, const vector_t* b);
matrix_t* matrix_vector_multiply(const matrix_t* a, const vector_t* b);
matrix_t* matrix_vector_divide(const matrix_t* a, const vector_t* b);

// matrix-matrix
matrix_t* matrix_matrix_elementwise_operation(
    const matrix_t* a, const matrix_t* b, float (*operation)(float, float)
);
matrix_t* matrix_matrix_add(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_subtract(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_multiply(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_divide(const matrix_t* a, const matrix_t* b);

//
matrix_t* matrix_transpose(const matrix_t* matrix);
float     matrix_dot_product(const matrix_t* a, const matrix_t* b);

#endif // ALT_MATRIX_H
