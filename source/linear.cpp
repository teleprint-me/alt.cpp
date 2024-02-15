/*
 * alt.cpp/source/linear.cpp
 *
 * Copyright © 2024 Austin Berrio
 *
 * linear(x, m, b) usage:
 *
 * {
 *   float x = 3.0;
 *   float m = 2.0;
 *   float b = 1.0;
 *   float y = linear(x, m, b);
 *   printf("For x = %.2f, y = %.2f\n", x, y);
 * }
 *
 * mean_square_error(X, Y, size, m, b) usage:
 *
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
 * solve for partial derivative
 * 1. Slope-intercept Form = SIF = y = m * x + b
 * 2. Mean Squared Error = MSE = (1 / n) * Σ(y' - y)^2
 * 3. Substitute the label y with SIF = (1 / n) * Σ(y' - (m * x + b))^2
 * 4. Find the partial derivatives of MSE with respect to both slope and y-intercept.
 * Differentiation: Process of finding derivatives
 * Chain rule: Differentiate composite function, e.g. f(g(x))
 * slope = lim h->0 (f(x + h) - f(x) / h)
 *
 *    mse = 1/n * Σ(y' - (m * x + b))^2
 *
 *    ∂e/∂m = 1/n * Σ 2 * (y' - (m * x + b)) (-x)
 *    ∂e/∂m = -2/n * Σx(y' - (m * x) - b))
 *
 *    ∂e/∂b = 2/n * Σ(y' - (m * x + b)) (-1)
 *    ∂e/∂b = -2/n * Σ(y' - (m * x) - b))
 *
 * Goal: Fit dataset of labels and predictions to a line.
 */
// attempt to avoid relying on math.h unless otherwise necessary. it's okay to use math functions if
// it helps avoid boilerplate and/or complicated algorithms and constants, e.g. e, pi, sqrt, etc.
#include <stdio.h>

// equation of a line
// y = m(X) + b
float slope_intercept_form(float x, float m, float b) { return m * x + b; }

// error in distance of a set of points on a line
// mse = Σ(y' - y)^2
float mean_square_error(float X[], float Y[], size_t size, float m, float b) {
    float error = 0.0f;

    for (size_t i = 0; i < size; i++) {
        float y_prime = slope_intercept_form(X[i], m, b);
        float diff    = y_prime - Y[i];
        error += diff * diff; // Using diff * diff for efficiency, as you suggested
    }

    return size > 0 ? error / size : 0.0f; // Guard against division by zero
}

// Calculates the partial derivative of the error with respect to m
// ∂e/∂m = -2/n * Σx(y' - (m * x) - b))
float partial_derivative_m(float X[], float Y[], size_t size, float m, float b) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; i++) {
        sum += X[i] * (slope_intercept_form(X[i], m, b) - Y[i]);
    }
    return -2.0f / size * sum;
}

// Calculates the partial derivative of the error with respect to b
// ∂e/∂b = -2/n * Σ(y' - (m * x) - b))
float partial_derivative_b(float X[], float Y[], size_t size, float m, float b) {
    float sum = 0.0f;
    for (size_t i = 0; i < size; i++) {
        sum += (slope_intercept_form(X[i], m, b) - Y[i]);
    }
    return -2.0f / size * sum;
}
