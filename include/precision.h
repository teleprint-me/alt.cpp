/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/precision.h
 *
 * @brief A simple and easy-to-use API for handling precision in C
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef ALT_PRECISION_H
#define ALT_PRECISION_H

#include <assert.h>
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
    TYPE_MAX_COUNT,  // Number of data types
} data_type_t;

// Data types for different precisions
typedef uint32_t float32_t;
typedef uint16_t float16_t;
typedef uint16_t bfloat16_t;
typedef uint8_t  float8_t;

// Union to map float to internal integer representation
typedef union {
    float32_t bits;
    float     value;
} float_data_t;

// Struct to represent different float types
typedef struct {
    float_data_t value;
    data_type_t  type;
} float_flex_t;

// Function declarations

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

// Encode and decode functions for 32-bit and 16-bit floats
float32_t encode_float32(float value);
float     decode_float32(float32_t bits);

float16_t encode_float16(float value);
float     decode_float16(float16_t bits);

bfloat16_t encode_bfloat16(float value);
float      decode_bfloat16(bfloat16_t bits);

float8_t encode_float8(float value);
float    decode_float8(float8_t bits);

#endif // ALT_PRECISION_H
