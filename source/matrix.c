/**
 * Copyright © 2024 Austin Berrio
 *
 * @file source/matrix.c
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

#include "../include/matrix.h"
#include "../include/logger.h"

#include <stdio.h>
#include <string.h>

// Matrix operations
matrix_t* matrix_create(size_t columns, size_t rows) {
    matrix_t* matrix = (matrix_t*) malloc(sizeof(matrix_t));
    if (NULL == matrix) {
        fprintf(stderr, "Failed to allocate memory for matrix_t.\n");
        return NULL;
    }

    matrix->elements = (float**) malloc(rows * sizeof(float*));
    if (NULL == matrix->elements) {
        fprintf(stderr, "Failed to allocate memory for matrix rows.\n");
        free(matrix);
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i) {
        matrix->elements[i] = (float*) malloc(columns * sizeof(float));
        if (NULL == matrix->elements[i]) {
            fprintf(stderr, "Failed to allocate memory for matrix columns.\n");
            // Free previously allocated rows
            for (size_t j = 0; j < i; ++j) {
                free(matrix->elements[j]);
            }
            free(matrix->elements);
            free(matrix);
            return NULL;
        }
        memset(matrix->elements[i], 0, columns * sizeof(float));
    }

    matrix->columns = columns;
    matrix->rows    = rows;

    return matrix;
}

void matrix_free(matrix_t* matrix) {
    if (NULL == matrix) {
        fprintf(stderr, "Cannot free a NULL matrix.\n");
        return;
    }

    if (matrix->elements) {
        free(matrix->elements);
    }

    free(matrix);
}
