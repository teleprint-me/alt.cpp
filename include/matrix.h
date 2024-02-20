/*
 * alt.cpp/include/matrix.h
 *
 * Copyright © 2024 Austin Berrio
 *
 * Overview:
 * This library provides a comprehensive and efficient Vector and Matrix API designed for high
 * performance and ease of use in scientific computing, data analysis, and machine learning
 * applications. It emphasizes simplicity, speed, and flexibility, leveraging pure C for core
 * operations to ensure maximum compatibility and performance. Advanced mathematical operations and
 * transformations are supported with minimal dependency on external libraries, adhering to a
 * philosophy of lightweight and focused design.
 *
 * Note:
 * - Prefixing related objects, functions, etc., has been adopted to enhance code readability and
 * assist with autocomplete features in various development environments. This convention is
 * intended to streamline development workflows and facilitate quick integration into existing
 * projects.
 *
 * This API stands as a testament to the power and flexibility of C, aiming to provide developers
 * with a robust toolkit for tackling a broad spectrum of computational challenges.
 *
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
 * @brief Represents a matrix in n-dimensional space.
 */
struct Matrix {
    // We have n (rows) x m (cols)
    // e.g. matrix = {{2, 2}, {1, 3}}; // n x m = 2 x 2 dimensions
    // need to track the number rows and cols
    // n (rows) represents the vectors the matrix
    // m (cols) resresents the y axis of the matrix?
    // problems arise with higher dimensions? How do account for differing dimensionialities?
    struct Vector* rows;

    size_t n_cols;
    // NOTE: Each vector has it's own size,
    // e.g. `vector->size` returns the number of elements in `vector->elements`.
    size_t n_rows;
};

// Life-cycle operations

struct Matrix* matrix_create(size_t n_rows, size_t n_cols);

struct Matrix* matrix_deep_copy(const struct Matrix* matrix);

struct Matrix* matrix_shallow_copy(const struct Matrix* matrix);

bool matrix_destroy(struct Matrix* matrix);

#endif // ALT_MATRIX_H
