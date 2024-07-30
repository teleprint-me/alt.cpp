
/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/precision.h
 *
 * @brief A simple and easy-to-use API in C for handling various floating-point
 *        precisions, including 32-bit (float), 16-bit (half precision float:
 *        IEEE-754 & bfloat16), 16-bit Google Brain format, and extended 8-bit
 *        floats.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 *
 * @note Official document
 * @ref https://ieeexplore.ieee.org/document/8766229
 *
 * @note 3rd party work
 * @ref shttps://geeksforgeeks.org/ieee-standard-754-floating-point-numbers/
 *
 * @note Source code
 * @ref /usr/include/c10/util/half.h
 * @ref https://github.com/Maratyszcza/FP16
 * @ref https://github.com/pytorch/pytorch/blob/main/c10/util/Half.h
 */

#ifndef ALT_PRECISION_H
#define ALT_PRECISION_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Default tolerance for floating-point comparisons
#define FLOAT_TOLERANCE 0.0001f

// Enumeration of data types
typedef enum {
    TYPE_FLOAT_F32,  // IEEE-754 32-bit precision
    TYPE_FLOAT_F16,  // IEEE-754 16-bit precision
    TYPE_FLOAT_BF16, // Google Brain bfloat16 precision
    TYPE_FLOAT_F8,   // Extended 8-bit precision
    TYPE_QUANT_K8,   // k-bit precision
    TYPE_QUANT_K4,   // k-bit precision
    TYPE_MAX_COUNT,  // Number of data types
} data_type_t;

// Data types for different precisions
typedef uint32_t float32_t;
typedef uint16_t float16_t;
typedef uint16_t bfloat16_t;
typedef uint8_t  float8_t;

/**
 * @brief A union representing a flexible floating-point representation,
 *        allowing access to both raw bit data and its corresponding value.
 *
 * @param bits  Raw 32-bit integer bit representation of the floating-point
 * number.
 * @param value The actual 32-bit floating-point value represented by this data
 * structure.
 */
typedef union {
    float32_t bits;
    float     value;
} float_data_t;

/**
 * @brief Represents a flexible floating point data structure capable of
 *        handling different precisions and formats.
 *
 * @param value  The actual floating-point value represented by this data
 * structure.
 * @param type   Indicates the specific precision or format (e.g., float32,
 * bfloat16)
 */
typedef struct {
    float_data_t value;
    data_type_t  type;
} float_flex_t;

/**
 * @brief Determines if two floating-point values are approximately equal within
 * specified tolerances.
 *
 * @param[in]   a       The first floating-point value.
 * @param[in]   b       The second floating-point value.
 * @param[in]   tolerance Tolerance for comparing values (default:
 * FLOAT_TOLERANCE).
 *
 * @return true if the absolute difference between 'a' and 'b' is within the
 * tolerance bounds, false otherwise.
 */
bool float_is_close(float a, float b, float tolerance);

/**
 * @brief Encodes a given float value into its corresponding 32-bit
 * representation.
 *
 * @param[in] value The floating point number to be encoded.
 *
 * @return The resulting encoded 32-bit integer representation of the input
 * value.
 */
float32_t encode_float32(float value);

/**
 * @brief Decodes a given 32-bit integer representation into its corresponding
 * float value.
 *
 * @param[in] bits The encoded 32-bit integer bit representation of the
 * floating-point number.
 *
 * @return The decoded actual 32-bit floating-point value represented by this
 * data structure.
 */
float decode_float32(float32_t bits);

/**
 * @brief Encodes a given float value into its corresponding 16-bit
 * representation (IEEE-754 half precision).
 *
 * @param[in] value The floating point number to be encoded.
 *
 * @return The resulting encoded 16-bit integer representation of the input
 * value.
 */
float16_t encode_float16(float value);

/**
 * @brief Decodes a given 16-bit integer representation into its corresponding
 * float value (IEEE-754 half precision).
 *
 * @param[in] bits The encoded 16-bit integer bit representation of the
 * floating-point number.
 *
 * @return The decoded actual 16-bit floating-point value represented by this
 * data structure.
 */
float decode_float16(float16_t bits);

/**
 * @brief Encodes a given float value into its corresponding Google Brain
 * bfloat16 representation (half precision).
 *
 * @param[in] value The floating point number to be encoded using Google Brain's
 * 16-bit format.
 *
 * @return The resulting encoded 16-bit integer representation of the input
 * value in bfloat16 format.
 */
bfloat16_t encode_bfloat16(float value);

/**
 * @brief Decodes a given bfloat16 integer representation into its corresponding
 * float value.
 *
 * @param[in] bits The encoded 16-bit bit representation of the floating-point
 * number in bfloat16 format.
 *
 * @return The decoded actual 32-bit floating-point value represented by this
 * data structure.
 */
float decode_bfloat16(bfloat16_t bits);

/**
 * @brief Encodes a given float value into its corresponding 8-bit
 * representation (Extended 8-bit floating point).
 *
 * @param[in] value The floating point number to be encoded.
 *
 * @return The resulting encoded 8-bit integer representation of the input
 * value.
 */
float8_t encode_float8(float value);

/**
 * @brief Decodes a given 8-bit integer representation into its corresponding
 * float value (Extended 8-bit floating point).
 *
 * @param[in] bits The encoded 8-bit bit representation of the floating-point
 * number.
 *
 * @return The decoded actual extended 8-bit floating-point value represented by
 * this data structure.
 */
float decode_float8(float8_t bits);

#endif // ALT_PRECISION_H
