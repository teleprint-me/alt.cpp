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
#include <stdio.h>

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
float slope_intercept_form(float x, float m, float b) { return m * x + b; }

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
        float y_prime = slope_intercept_form(X[i], m, b);
        float diff    = y_prime - Y[i];
        error += diff * diff; // Using diff * diff for efficiency
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
