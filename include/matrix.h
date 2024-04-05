/*
 * alt.cpp/include/matrix.h
 *
 * Copyright © 2024 Austin Berrio
 *
 * This library provides a Vector and Matrix API designed for high performance and ease of use in
 * scientific computing, data analysis, and machine learning applications. It leverages pure C for
 * core operations to ensure maximum compatibility and performance with a focus on simplicity,
 * speed, and flexibility.
 */
#ifndef ALT_MATRIX_H
#define ALT_MATRIX_H

#include "vector.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures

/**
 * Represents a matrix in n-dimensional space, where each row is a vector.
 * Ensures that all rows (vectors) have the same length to maintain consistency.
 */
typedef struct {
    struct Vector* rows;   // Pointer to an array of Vector structures representing rows
    size_t         n_rows; // Number of rows in the matrix
    size_t         n_cols; // Number of columns in each row (vector)
} Matrix;

// Life-cycle operations

/**
 * Creates a new matrix with the specified number of rows and columns.
 * Initializes all elements to zero.
 *
 * @param n_rows Number of rows.
 * @param n_cols Number of columns.
 * @return Pointer to the newly created matrix or NULL if allocation fails.
 */
Matrix* matrix_create(size_t n_rows, size_t n_cols);

/**
 * Creates a deep copy of a given matrix, duplicating all its elements.
 *
 * @param matrix Pointer to the matrix to be copied.
 * @return Pointer to the new matrix copy or NULL if allocation fails.
 */
Matrix* matrix_deep_copy(const Matrix* matrix);

/**
 * Creates a shallow copy of a given matrix. Only the matrix structure is duplicated, not the data.
 *
 * @param matrix Pointer to the matrix to be copied.
 * @return Pointer to the new matrix structure or NULL if allocation fails.
 */
Matrix* matrix_shallow_copy(const Matrix* matrix);

/**
 * Destroys a matrix, freeing its memory. Safely handles NULL pointers.
 *
 * @param matrix Pointer to the matrix to be destroyed.
 * @return true if the operation is successful, false otherwise.
 */
bool matrix_destroy(Matrix* matrix);

// Additional operations (placeholders for future implementation)

/**
 * Adds two matrices and returns the result.
 *
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 * @return Pointer to the result matrix or NULL if the operation fails.
 */
Matrix* matrix_add(const Matrix* a, const Matrix* b);

/**
 * Multiplies two matrices and returns the result.
 *
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 * @return Pointer to the result matrix or NULL if the operation fails.
 */
Matrix* matrix_multiply(const Matrix* a, const Matrix* b);

/**
 * Transposes a matrix and returns the result.
 *
 * @param matrix Pointer to the matrix to be transposed.
 * @return Pointer to the transposed matrix or NULL if the operation fails.
 */
Matrix* matrix_transpose(const Matrix* matrix);

#endif // ALT_MATRIX_H
