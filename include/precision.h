#ifndef ALT_PRECISION_H
#define ALT_PRECISION_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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
typedef uint8_t  float8_t;
typedef uint16_t bfloat16_t;

// Struct to define precision parameters
typedef struct {
    float inf;  // Upper bound (± ∞)
    float zero; // Lower bound (typically ± 0)

    uint32_t p;        // Precision (number of significand digits)
    uint32_t sign;     // Sign bit count
    uint32_t exponent; // Exponent bit count
    uint32_t mantissa; // Significand bit count

    int32_t e_bias; // Bias for the exponent
    int32_t e_max;  // Maximum exponent value (derived from e and e_bias)
    int32_t e_min;  // Minimum exponent value (derived from e and e_bias)
} precision_t;

// Union to map float to internal integer representation
typedef union {
    float32_t bits;
    float     value;
} float_data_t;

// Generalized float structure for various float types
typedef struct {
    // NOTE: This always consumes 32-bits
    union {
        float      f32;
        float32_t  uint32;
        float16_t  uint16;
        bfloat16_t uint16;
        float8_t   uint8;
    } value;

    data_type_t type;
} float_flex_t;

// Function declarations

/**
 * @brief Determines if two floating-point values are approximately equal within
 * specified tolerances.
 *
 * This function checks whether the absolute difference between 'a' and 'b' is
 * less than or equal to the tolerance bounds, defined by both a relative
 * threshold (default: 1e-03) and an absolute threshold (default: 0.0).
 *
 * @param[in]   a       The first floating-point value.
 * @param[in]   b       The second floating-point value.
 * @param[in]   relative Relative tolerance for comparing values (default:
 * 1e-03).
 * @param[in]   absolute Absolute tolerance for comparing values (default: 0.0).
 *
 * @return true if the absolute difference between 'a' and 'b' is within the
 * tolerance bounds, false otherwise.
 */
bool float_is_close(float a, float b, float relative, float absolute);

// Encode float value based on type
float_flex_t encode_float(float value, data_type_t dtype);
float        decode_float(const float_flex_t* flex);

// Encode standard 32-bit floating-point
float32_t encode_float32(float value);
float     decode_float32(const float32_t* flex);

// Encode standard 16-bit floating-point
float16_t encode_float16(float value);
float     decode_float16(const float16_t* flex);

// Encode extended 8-bit float
float8_t encode_float8(float value);
float    decode_float8(const float8_t* flex);

// Encode Google Brain 16-bit float
bfloat16_t encode_bfloat16(float value);
float      decode_bfloat16(const bfloat16_t* flex);

#endif // ALT_PRECISION_H
