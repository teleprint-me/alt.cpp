/*
 * alt.cpp/include/vector.h
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
 * Features:
 * - Dynamic creation and management of vectors and matrices with support for deep copying and
 * cloning.
 * - A suite of mathematical operations, including vector normalization, magnitude calculation, and
 * a variety of element-wise operations for flexible data manipulation.
 * - Advanced vector operations such as dot and cross products, alongside essential functions for
 * coordinate transformation, providing the tools necessary for complex spatial and algebraic
 * calculations.
 * - Specialized functions for machine learning applications, including softmax activation and
 * cross-entropy loss calculation, facilitating the development of algorithms and models directly in
 * C.
 * - Utility functions for common tasks such as vector scaling and distance calculation, enhancing
 * the library's applicability to a wide range of computational problems.
 *
 * Usage Guidelines:
 * - This API is designed to be intuitive for users familiar with vector and matrix operations.
 * Function names and parameters follow conventional mathematical notation where possible.
 * - Users are encouraged to leverage the element-wise operation function to extend the library's
 * functionality and implement custom operations as needed.
 * - While the library aims to operate purely in C, certain operations may require linking against
 * standard math libraries (e.g., -lm on Unix-like systems) for complex mathematical functions.
 * - For best performance, attention should be paid to memory management practices, especially in
 * applications requiring intensive computational resources or running on constrained hardware.
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
#ifndef ALT_VECTOR_H
#define ALT_VECTOR_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures

/**
 * @brief Represents a vector in n-dimensional space.
 */
struct Vector {
    /**
     * @brief Pointer to the array storing the n-dimensional vector elements.
     */
    float* elements;

    /**
     * @brief Number of elements in the vector, representing its dimensionality.
     */
    size_t size;
};

// Life-cycle operations

/**
 * @brief Creates a new vector with the specified size and initializes its elements to zero.
 *
 * The `vector_create` function dynamically allocates memory for a new vector structure and its
 * elements array. If memory allocation fails, an error message is logged, and the function returns
 * NULL. Otherwise, the function initializes all elements of the vector to zero and sets its size.
 *
 * @param size The size of the vector to be created.
 * @return A pointer to the newly created vector on success, or NULL if memory allocation failed.
 */
struct Vector* vector_create(size_t size);

/**
 * @brief Creates a deep copy of the input vector.
 *
 * This function allocates memory for a new vector and copies the elements of the input vector into
 * the new memory space. The new vector is completely independent of the input vector and can be
 * modified without affecting the original vector.
 *
 * @param vector The vector to be copied.
 * @return A pointer to the newly created deep copy of the input vector. Returns NULL if memory
 * allocation fails or if the input vector is NULL.
 */
struct Vector* vector_deep_copy(const struct Vector* vector);

/**
 * @brief Creates a shallow copy of the input vector.
 *
 * This function creates a new vector structure and assigns the pointer to the elements array of
 * the input vector to the new vector. As a result, both the original and copied vectors share the
 * same underlying elements array. Modifying the elements of one vector will affect the other.
 *
 * @param vector The vector to be copied.
 * @return A pointer to the newly created shallow copy of the input vector. Returns NULL if memory
 * allocation fails or if the input vector is NULL.
 */
struct Vector* vector_shallow_copy(const struct Vector* vector);

/**
 * @brief Destroys a vector and frees its allocated memory.
 *
 * The `vector_destroy` function frees the memory allocated for the input vector and its elements
 * array. If the input vector or its elements array is NULL, an error message is logged, and the
 * function returns false.
 *
 * @param vector A pointer to the vector to be destroyed.
 * @return true if the vector was successfully destroyed and memory freed, false otherwise.
 */

bool vector_destroy(struct Vector* vector);

// Mathematical operations

/**
 * @brief Calculates the magnitude of a vector.
 *
 * The `vector_magnitude` function computes the Euclidean magnitude of the input vector by summing
 * the squares of its elements and taking the square root of the result. This represents the length
 * of the vector in n-dimensional space.
 *
 * @param vector A pointer to the vector whose magnitude is to be calculated.
 * @return The magnitude of the input vector.
 */
float vector_magnitude(const struct Vector* vector);

/**
 * @brief Normalizes a vector to produce a unit vector.
 *
 * The `vector_normalize` function normalizes the input vector by dividing each of its elements by
 * its magnitude, resulting in a unit vector with the same direction but a magnitude of 1. If the
 * input vector has a magnitude of zero, an error message is logged, and NULL is returned.
 *
 * If `inplace` is true, the normalization is performed in-place, modifying the input vector. If
 * `inplace` is false, a new vector is created to store the normalized result, leaving the input
 * vector unchanged.
 *
 * @param vector A pointer to the vector to be normalized.
 * @param inplace A boolean indicating whether to perform the normalization in-place or not.
 * @return A pointer to the normalized unit vector on success, or NULL if memory allocation failed
 * or if the input vector has zero magnitude.
 */
struct Vector* vector_normalize(struct Vector* vector, bool inplace);

/**
 * @brief Calculates the Euclidean distance between two n-dimensional vectors.
 *
 * This function calculates the Euclidean distance between two given vectors `a` and `b`, where each
 * vector is represented by a pointer to its underlying elements and its size. The function first
 * checks if the sizes of both vectors match. If they don't, an error message is logged and the
 * function returns NaN as the result. Otherwise, it calculates the sum of squared differences
 * between the corresponding components of the two vectors. Finally, the function takes the square
 * root of the sum to find the Euclidean distance and returns it as the result.
 *
 * @param a A pointer to the first vector.
 * @param b A pointer to the second vector.
 * @return The Euclidean distance between vectors `a` and `b`. If the sizes of the vectors do not
 * match, the function returns NaN.
 */
float vector_distance(const struct Vector* a, const struct Vector* b);

/**
 * @brief Scales a vector by a given scalar value.
 *
 * This function scales a vector by a scalar, either modifying the original vector (in-place)
 * or creating a new scaled vector (out-of-place), based on the `inplace` parameter.
 * If `vector` is NULL, the function returns NULL to indicate an error.
 * For out-of-place scaling, if memory allocation for the new vector fails, the function logs
 * an error and returns NULL.
 *
 * @param vector A pointer to the vector to scale.
 * @param scalar The scalar value by which to scale the vector's elements.
 * @param inplace If true, scales the original vector in-place. If false, returns a new scaled
 * vector.
 * @return A pointer to the scaled vector (which may be the same as `vector` if `inplace` is true),
 *         or NULL if the operation could not be performed.
 */
struct Vector* vector_scale(struct Vector* vector, float scalar, bool inplace);

/**
 * @brief Calculates the mean (average) of the elements in a vector.
 *
 * @param vector The vector whose mean is to be calculated.
 * @return The mean of the vector's elements. Returns NAN if the vector is NULL, empty, or contains
 * invalid (NaN) elements.
 */
float vector_mean(struct Vector* vector);

/**
 * @brief Clips the elements of a vector to be within the specified range.
 *
 * @param vector The input vector to be clipped.
 * @param min The minimum value for clipping.
 * @param max The maximum value for clipping.
 * @param inplace If true, performs the clipping operation in place on the input vector.
 *                If false, creates a new vector with clipped elements.
 * @return A pointer to the clipped vector if inplace is false, otherwise returns the input vector.
 *         Returns NULL if the input vector is NULL.
 */
struct Vector* vector_clip(struct Vector* vector, float min, float max, bool inplace);

// Element wise operations

// Helper functions for element-wise operations
float add(float x, float y);
float subtract(float x, float y);
float multiply(float x, float y);
float divide(float x, float y);

/**
 * @brief Performs an element-wise operation on two input vectors.
 *
 * The `perform_elementwise_operation` function takes two input vectors `a` and `b` and applies the
 * specified element-wise operation to each corresponding pair of elements. The operation is defined
 * by a function pointer `operation` which takes two floats and returns a float.
 *
 * If the sizes of the input vectors do not match, an error message is logged, and the function
 * returns NULL. Otherwise, the function dynamically allocates memory for the resultant vector `c`
 * and performs the element-wise operation, storing the results in `c`.
 *
 * @param a The first input vector.
 * @param b The second input vector.
 * @param operation A function pointer to the element-wise operation to be performed.
 * @return A pointer to the resultant vector on success, or NULL if an error occurred.
 */
struct Vector* perform_elementwise_operation(
    const struct Vector* a, const struct Vector* b, float (*operation)(float, float)
);

// Element-wise operation wrappers

/**
 * @brief Performs element-wise addition between two n-dimensional vectors.
 */
struct Vector* vector_add(const struct Vector* a, const struct Vector* b);

/**
 * @brief Performs element-wise subtraction between two n-dimensional vectors.
 */
struct Vector* vector_subtract(const struct Vector* a, const struct Vector* b);

/**
 * @brief Performs element-wise multiplication between two n-dimensional vectors.
 */
struct Vector* vector_multiply(const struct Vector* a, const struct Vector* b);

/**
 * @brief Performs element-wise division between two n-dimensional vectors.
 */
struct Vector* vector_divide(const struct Vector* a, const struct Vector* b);

// Vector products

/**
 * @brief Computes the dot product of two input vectors.
 *
 * The `vector_dot_product` function calculates the dot product of two input vectors `a` and `b`,
 * which represents the cosine of the angle between the two vectors multiplied by their
 * magnitudes. The dot product is computed by summing the products of the corresponding elements
 * of the input vectors.
 *
 * If the sizes of the input vectors do not match, an error message is logged, and `NAN` (Not a
 * Number) is returned.
 *
 * @param a A pointer to the first input vector.
 * @param b A pointer to the second input vector.
 * @return The dot product of the input vectors on success, or `NAN` if the input vector sizes do
 * not match.
 */
float vector_dot_product(const struct Vector* a, const struct Vector* b);

/**
 * @brief Computes the cross product of two 3-dimensional vectors.
 *
 * The `vector_cross_product` function calculates the cross product of two input 3-dimensional
 * vectors `a` and `b`, which represents a vector perpendicular to both input vectors. The cross
 * product is only defined for 3-dimensional vectors.
 *
 * If either of the input vectors is not 3-dimensional, an error message is logged, and NULL is
 * returned. Otherwise, the function allocates memory for the resulting cross product vector and
 * computes its components based on the input vectors.
 *
 * @param a A pointer to the first input 3-dimensional vector.
 * @param b A pointer to the second input 3-dimensional vector.
 * @return A pointer to the resulting cross product vector on success, or NULL if the input vectors
 * are not 3-dimensional or if memory allocation failed.
 */
struct Vector* vector_cross_product(const struct Vector* a, const struct Vector* b);

// Coordinate transformations
struct Vector* vector_polar_to_cartesian(const struct Vector* polar_vector);
struct Vector* vector_cartesian_to_polar(const struct Vector* cartesian_vector);

// Advanced operations

/**
 * @brief Computes the softmax function of a vector.
 *
 * The `vector_softmax` function calculates the softmax of the input vector, which is a
 * normalization function that converts a vector of arbitrary real values into a vector of
 * probabilities. The softmax function is computed by taking the exponential of each element in the
 * input vector, dividing each exponential by the sum of all exponentials, and storing the result
 * in the output vector.
 *
 * If memory allocation fails during the calculation process, NULL is returned. Otherwise, the
 * function returns a pointer to the resulting softmax vector.
 *
 * @param vector A pointer to the input vector.
 * @return A pointer to the resulting softmax vector on success, or NULL if memory allocation
 * failed.
 */
struct Vector* vector_softmax(const struct Vector* vector);

/**
 * @brief Calculates the cross-entropy loss between the predicted values and the target values.
 *
 * Cross-entropy loss is a measure of the difference between two probability distributions.
 * It is commonly used in machine learning for binary classification problems.
 * The formula used here is `−∑ y log(p)`, where `y` is the target value and `p` is the predicted
 * value, both clipped to ensure they are within a valid range for logarithmic calculations.
 *
 * @param prediction The predicted values (probability distribution).
 * @param target The target values (true labels).
 * @param epsilon A small value added to predicted values to prevent taking the logarithm of zero.
 * @return The cross-entropy loss between the predicted and target values.
 *         Returns NaN if either prediction or target is NULL, or if their sizes do not match.
 */
float vector_cross_entropy(
    const struct Vector* prediction, const struct Vector* target, float epsilon
);

#endif // ALT_VECTOR_H
