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

#include <stdbool.h>
#include <stdlib.h>

// Structures

/**
 * @brief A structure representing a 2-dimensional matrix.
 *
 * A matrix is a rectangular array of rows and columns representing a
 * 2-dimensional space. This structure stores the number of rows and columns,
 * along with a two-dimensional dynamic array of floating-point values, which
 * represent the components of the matrix.
 *
 * @param elements A two-dimensional pointer to an array of floats, representing
 * the matrix elements.
 * @param columns  The number of columns (width) of the matrix.
 * @param rows     The number of rows (height) of the matrix.
 */
typedef struct {
    float**
        elements;   ///< Two-dimensional array representing the matrix elements.
    size_t columns; ///< The number of columns (width) of the matrix.
    size_t rows;    ///< The number of rows (height) of the matrix.
} matrix_t;

// Life-cycle operations

/**
 * @brief Creates a new matrix with the specified number of rows and columns.
 * Initializes all elements to zero.
 *
 * @param cols Number of columns.
 * @param rows Number of rows.
 *
 * @return Pointer to the newly created matrix or NULL if allocation fails.
 */
matrix_t* matrix_create(size_t columns, size_t rows);

/**
 * Free its memory. Safely handles NULL pointers.
 *
 * @param matrix Pointer to the matrix to be destroyed.
 * @return true if the operation is successful, false otherwise.
 */
void matrix_free(matrix_t* matrix);

/**
 * Creates a deep copy of a given matrix, duplicating all its elements.
 *
 * @param matrix Pointer to the matrix to be copied.
 * @return Pointer to the new matrix copy or NULL if allocation fails.
 */
matrix_t* matrix_deep_copy(const matrix_t* matrix);

/**
 * Creates a shallow copy of a given matrix. Only the matrix structure is
 * duplicated, not the data.
 *
 * @param matrix Pointer to the matrix to be copied.
 * @return Pointer to the new matrix structure or NULL if allocation fails.
 */
matrix_t* matrix_shallow_copy(const matrix_t* matrix);

// Additional operations (placeholders for future implementation)

/**
 * Adds two matrices and returns the result.
 *
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 * @return Pointer to the result matrix or NULL if the operation fails.
 */
matrix_t* matrix_add(const matrix_t* a, const matrix_t* b);

/**
 * Multiplies two matrices and returns the result.
 *
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 * @return Pointer to the result matrix or NULL if the operation fails.
 */
matrix_t* matrix_multiply(const matrix_t* a, const matrix_t* b);

/**
 * Transposes a matrix and returns the result.
 *
 * @param matrix Pointer to the matrix to be transposed.
 * @return Pointer to the transposed matrix or NULL if the operation fails.
 */
matrix_t* matrix_transpose(const matrix_t* matrix);

#endif // ALT_MATRIX_H
