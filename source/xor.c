/**
 * xor.c
 *
 * The XOR problem is a simple boolean logic function that takes two binary
 * inputs (0 or 1) and outputs the exclusive OR of those values:
 *
 * XOR(A, B) = A ⊕ B
 *
 * This problem serves as an example for creating a basic neural network model
 * to solve this logical operation using matrix multiplication, addition,
 * activation functions, loss calculation, backpropagation (gradient descent),
 * and tensor creation.
 *
 * Operations required to solve the xor problem:
 *
 * 1. Matrix Multiplication
 * 2. Element-wise Addition
 * 3. Sigmoid Activation Function
 * 4. Derivative of the Sigmoid Activation Function
 * 5. Loss Calculation
 * 6. Backpropagation - Forward Propagation
 * 7. Gradient Descent (Backpropagation - Update Weights)
 *
 */

#include "logger.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief A structure representing a 2-dimensional matrix.
 *
 * A matrix is a rectangular array of rows and columns representing a
 * 2-dimensional space. This structure stores the number of rows and columns,
 * along with a two-dimensional dynamic array of floating-point values, which
 * represent the components of the matrix.
 *
 * @param elements A N-dimensional pointer to an array of floats, representing
 * the matrix elements.
 * @param columns  The number of columns (width) of the matrix.
 * @param rows     The number of rows (height) of the matrix.
 */
typedef struct {
    float* elements; ///< N-dimensional array representing the matrix elements.

    size_t rows;    ///< The number of rows (height) of the matrix.
    size_t columns; ///< The number of columns (width) of the matrix.
} matrix_t;

/**
 * @brief Creates a new matrix with the specified number of rows and columns.
 * Initializes all elements to zero.
 *
 * @param rows Number of rows.
 * @param columns Number of columns.
 *
 * @return Pointer to the newly created matrix or NULL if allocation fails.
 */
matrix_t* matrix_create(size_t rows, size_t columns);

/**
 * matrix_free - Free a previously allocated matrix and its data.
 *
 * @mat: The matrix to be freed.
 */
void matrix_free(matrix_t* mat);

matrix_t* matrix_mat_mul(matrix_t* a, matrix_t* b);

// Matrix operations
matrix_t* matrix_create(size_t rows, size_t columns) {
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
