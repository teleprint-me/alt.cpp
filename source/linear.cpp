/*
 * alt.cpp/source/linear.cpp
 *
 * Copyright © 2024 Austin Berrio
 *
 * This file contains functions for linear regression.
 * It includes functions for calculating slope-intercept form, mean squared error,
 * and partial derivatives with respect to slope and y-intercept.
 *
 * Usage examples:
 *
 * float slope_intercept_form(float x, float m, float b)
 *   - Calculates the y-value in the slope-intercept form of a line.
 *     - y = ((m * x) + b)
 * {
 *   float x = 3.0;
 *   float m = 2.0;
 *   float b = 1.0;
 *   float y = linear(x, m, b);
 *   printf("For x = %.2f, y = %.2f\n", x, y);
 * }
 *
 * float mean_square_error(float X[], float Y[], size_t size, float m, float b)
 *   - Calculates the mean squared error (MSE) for a set of points on a line.
 *     - mse = (1 / n) * Σ(y' - ((m * x) + b))^2
 * {
 *   float X[] = {1, 2, 3};
 *   float Y[] = {2, 4, 6};
 *   size_t size = sizeof(X) / sizeof(X[0]);
 *   // Size calculation is valid here because it's in the same scope as the array declaration.
 *   float m = 2.0;
 *   float b = 0.0;
 *   float mse = mean_square_error(X, Y, size, m, b);
 *   printf("MSE: %f\n", mse);
 * }
 *
 * float partial_derivative_m(float X[], float Y[], size_t size, float m, float b)
 *   - Calculates the partial derivative of the mean squared error with respect to the slope.
 *     - ∂e/∂m = 1/n * Σ 2 * (y' - (m * x + b)) (-x)
 *     - ∂e/∂m = -2/n * Σx(y' - (m * x) - b))
 *
 * float partial_derivative_b(float X[], float Y[], size_t size, float m, float b)
 *   - Calculates the partial derivative of the mean squared error with respect to the y-intercept.
 *     - ∂e/∂b = 2/n * Σ(y' - (m * x + b)) (-1)
 *     - ∂e/∂b = -2/n * Σ(y' - (m * x) - b))
 *
 * IMPORTANT NOTES:
 *   - Differentiation is the process of finding derivatives.
 *   - The chain rule is used to differentiate composite functions, e.g., f(g(x)).
 *   - Slope is defined as the limit of the function as h approaches 0:
 *     - slope = lim h->0 (f(x + h) - f(x)) / h
 *
 *   - Slope-intercept Form (SIF) represents a line as y = m * x + b.
 *   - Mean Squared Error (MSE) measures the average squared difference between predicted and actual
 *     values:
 *     - MSE = (1 / n) * Σ(y' - y)^2
 *
 *   - To calculate the MSE, substitute the label y with the SIF equation:
 *     - (1 / n) * Σ(y' - (m * x + b))^2
 *
 *   - The goal is to fit a dataset of labels and predictions to a line.
 *
 * It is beneficial to avoid reliance on math.h unless necessary to avoid complexity and excessive
 * use of constants like e, pi, sqrt, etc. Use math functions when they simplify algorithms or
 * computations. Stick to C as much as possible and only leverage C++ features when absolutely
 * necessary. This approach helps maintain simplicity and compatibility across different platforms
 * and compilers.
 *
 */

#include "../include/logger.h"
#include "../include/vector.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// technically, we just have scalars, vectors, and matrices
// so we only have a number (typically a float); a point on a line?
// list of numbers; the standard vector, e.g. [i_1, i_2, i_3, ..., i_n]
// a "grid" of numbers; the standard matrix, e.g. [[n_1, n_2], [n_3, n_4]]
// the tricky part is managing the coordinates and dimensions

struct Params {
    float slope;
    float intercept;
};

struct LinearModel {
    struct Vector* X;
    struct Vector* Y;
    struct Params* params;
    float          learning_rate;
    size_t         iterations;
};

struct LinearModel* create_linear_model(size_t size, float learning_rate, size_t iterations) {
    struct LinearModel* linear_model = (struct LinearModel*) malloc(sizeof(struct LinearModel));

    linear_model->X             = vector_create(size);
    linear_model->Y             = vector_create(size);
    linear_model->learning_rate = learning_rate; // 1 * 10^-5
    linear_model->slope         = 1.0f;
    linear_model->intercept     = 1.0f;

    return linear_model;
}

bool destroy_linear_model(struct LinearModel* linear_model) {
    if (!linear_model) {
        return false;
    }

    // Free the vectors if they were allocated
    if (linear_model->X) {
        free(linear_model->X->elements);
        free(linear_model->X);
    }
    if (linear_model->Y) {
        free(linear_model->Y->elements);
        free(linear_model->Y);
    }

    // Free the Params struct
    free(linear_model->params);

    // Finally, free the LinearModel itself
    free(linear_model);
    return true;
}

/**
 * @brief Calculates the y-value in the slope-intercept form of a line.
 *
 * Calculates the equation of a line.
 *   y = (m * x) + b
 *
 * @param x The x-value.
 * @param m The slope.
 * @param b The y-intercept.
 * @return The calculated y-value.
 */
float slope_intercept_form(float x, float m, float b) {
    return m * x + b;
}

/**
 * @brief Calculates the mean squared error (MSE) for a set of points on a line.
 *
 * Calculates the error in the distance of a set of points on a line.
 *   mse = (1 / n) * Σ(y' - ((m * x) + b))^2
 *
 * @param X An array of x-values.
 * @param Y An array of corresponding y-values.
 * @param size The size of the arrays.
 * @param m The slope of the line.
 * @param b The y-intercept of the line.
 * @return The mean squared error.
 */
float mean_square_error(float X[], float Y[], size_t size, float m, float b) {
    float error = 0.0f;

    for (size_t i = 0; i < size; i++) {
        float y_prime  = slope_intercept_form(X[i], m, b);
        float diff     = y_prime - Y[i];
        error         += diff * diff; // Using diff * diff for efficiency
    }

    return size > 0 ? error / size : 0.0f; // Guard against division by zero
}

/**
 * @brief Calculates the partial derivative of the mean squared error with respect to the slope.
 *
 * Calculates the partial derivative of the error with respect to m
 *   ∂e/∂m = -2/n * Σx(y' - ((m * x) + b))
 *
 * @param X An array of x-values.
 * @param Y An array of corresponding y-values.
 * @param size The size of the arrays.
 * @param m The slope of the line.
 * @param b The y-intercept of the line.
 * @return The partial derivative with respect to the slope.
 */
float partial_derivative_m(float X[], float Y[], size_t size, float m, float b) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; i++) {
        sum += X[i] * (slope_intercept_form(X[i], m, b) - Y[i]);
    }
    return -2.0f / size * sum;
}

/**
 * @brief Calculates the partial derivative of the mean squared error with respect to the
 * y-intercept.
 *
 * Calculates the partial derivative of the error with respect to b
 *   ∂e/∂b = -2/n * Σ(y' - (m * x) - b))
 *
 * @param X An array of x-values.
 * @param Y An array of corresponding y-values.
 * @param size The size of the arrays.
 * @param m The slope of the line.
 * @param b The y-intercept of the line.
 * @return The partial derivative with respect to the y-intercept.
 */
float partial_derivative_b(float X[], float Y[], size_t size, float m, float b) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; i++) {
        sum += (slope_intercept_form(X[i], m, b) - Y[i]);
    }
    return -2.0f / size * sum;
}

float fit_linear_regression(
    float X[], float Y[], size_t size, float learning_rate, size_t iterations
) {
    float  m = 1.0f;
    float  b = 1.0f;
    size_t n = size;

    float mse, dedm, dedb;

    for (size_t i = 0; i < iterations; i++) {
        mse  = mean_square_error(X, Y, size, m, b);
        dedm = partial_derivative_m(X, Y, size, m, b);
        dedb = partial_derivative_b(X, Y, size, m, b);

        m = m - learning_rate * dedm;
        b = b - learning_rate * dedb;
    }

    return {m, b}; // we need to create a struct to track everything the right way
}
