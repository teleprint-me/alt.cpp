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

matrix_t* matrix_create(const size_t rows, const size_t columns) {
    matrix_t* matrix = (matrix_t*) malloc(sizeof(matrix_t));
    if (NULL == matrix) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for matrix_t.\n");
        return NULL;
    }

    // Allocate a single block of memory for the matrix elements
    matrix->elements = (float*) malloc(rows * columns * sizeof(float));
    if (NULL == matrix->elements) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for matrix elements.\n");
        free(matrix);
        return NULL;
    }

    // Initialize all elements to zero
    memset(matrix->elements, 0, rows * columns * sizeof(float));

    matrix->rows    = rows;
    matrix->columns = columns;

    return matrix;
}

void matrix_free(matrix_t* matrix) {
    if (NULL == matrix) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Cannot free a NULL matrix.\n");
        return;
    }

    if (matrix->elements) {
        free(matrix->elements);
    }

    free(matrix);
}
