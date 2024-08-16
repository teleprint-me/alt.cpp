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

float matrix_get_element(
    const matrix_t* matrix, const size_t row, const size_t column
) {
    return matrix->elements[row * matrix->columns + column];
}

void matrix_set_element(
    matrix_t* matrix, const size_t row, const size_t column, const float value
) {
    matrix->elements[row * matrix->columns + column] = value;
}

size_t matrix_elements(const matrix_t* matrix) {
    return matrix->rows * matrix->columns;
}

bool matrix_is_zero(const matrix_t* matrix) {
    for (size_t i = 0; i < matrix->rows * matrix->columns; i++) {
        if (matrix->elements[i] != 0.0f) {
            return false;
        }
    }
    return true;
}

bool matrix_is_square(const matrix_t* matrix) {
    return matrix->rows == matrix->columns;
}

bool matrix_is_transposed(const matrix_t* matrix) {
    return matrix->is_transposed;
}

bool matrix_is_identity(const matrix_t* matrix) {
    if (!matrix_is_square(matrix)) {
        return false;
    }

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->columns; j++) {
            float value = matrix->elements[i * matrix->columns + j];
            if ((i == j && value != 1.0f) || (i != j && value != 0.0f)) {
                return false;
            }
        }
    }
    return true;
}
