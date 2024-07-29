/**
 * Copyright © 2024 Austin Berrio
 *
 * @file source/precision.c
 *
 * @brief A simple and easy-to-use API for handling precision in C
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "../include/precision.h"
#include <math.h>
#include <stdint.h>

// Determines if two floating-point values are approximately equal within
// specified tolerances.
bool float_is_close(float a, float b, float tolerance /*= FLOAT_TOLERANCE*/) {
    return fabsf(a - b)
           <= fmaxf(tolerance * fmaxf(fabsf(a), fabsf(b)), tolerance);
}

// Function to encode a float into its IEEE-754 binary32 representation
float32_t encode_float32(float value) {
    float_data_t f32;
    f32.value = value;
    return f32.bits;
}

// Function to decode an IEEE-754 binary32 representation into a float
float decode_float32(float32_t bits) {
    float_data_t f32;
    f32.bits = bits;
    return f32.value;
}

/*
 * Convert a 32-bit floating-point number in IEEE single-precision format to a
 * 16-bit floating-point number in IEEE half-precision format, in bit
 * representation.
 */
uint16_t encode_float16(float value) {
    // NOTE: ??? -> details and rationale unknown -> what and why

    // 32-bit Upper bound -> 0_1110_1111 -> 2_004_877_312
    const float scale_to_inf  = decode_float(UINT32_C(0x77800000));
    // 32-bit Lower bound -> 0_0001_0001 -> 142_606_336
    const float scale_to_zero = decode_float(UINT32_C(0x08800000));

    // ???
    const float saturated_f = fabsf(value) * scale_to_inf;
    // ???
    float       base        = saturated_f * scale_to_zero;

    // 32-bit encoded value
    const uint32_t f      = encode_float(value);
    // shift the float value to the left by a multiple of 2
    // ???
    const uint32_t shl1_f = f + f;
    // 32-bit sign is always the first bit -> 1_0000_0000 -> 2_147_483_648
    const uint32_t sign   = f & UINT32_C(0x80000000);
    // 32-bit exponent bias -> 1_1111_1110 -> 4_278_190_080
    uint32_t       bias   = shl1_f & UINT32_C(0xFF000000);
    // is bias exceeding limit? upper limit -> 0_1110_0010 -> 1_895_825_408
    if (bias < UINT32_C(0x71000000)) {
        // if yes, set to upper bound of 0_1110_0010
        bias = UINT32_C(0x71000000);
    }

    // the base is the lower half of the exponent
    // 0_0000_1111 -> 125_829_120
    base = decode_float((bias >> 1) + UINT32_C(0x07800000)) + base;
    // get the bits for the base, e.g. 0_0000_1111
    const uint32_t bits          = encode_float(base);
    // 32-bit representation of a 16-bit shift of the exponent
    // 32-bit to 16-bit -> 0_1_1111_00_0000_0000 -> 31_744
    // 0 -> sign-bit, 1_1111 -> exponent bits, 00_0000_0000 -> mantissa bits
    const uint32_t exp_bits      = (bits >> 13) & UINT32_C(0x00007C00);
    // ???
    // ??? -> 0_0_0011_11_1111_1111 -> 4095
    const uint32_t mantissa_bits = bits & UINT32_C(0x00000FFF);
    // ???
    const uint32_t nonsign       = exp_bits + mantissa_bits;
    // shift 32-bit exponent -> 0xFF000000 -> 1_1111_1110 -> 4_278_190_080
    // 16-bit representation -> 0x7E00 -> 0_1_1111_10_0000_0000 -> 32_256
    return (sign >> 16)
           | (shl1_f > UINT32_C(0xFF000000) ? UINT16_C(0x7E00) : nonsign);
}

/*
 * Convert a 16-bit floating-point number in IEEE half-precision format to a
 * 32-bit floating-point number in IEEE single-precision format.
 */
float decode_float16(uint16_t bits) {
    const uint32_t f      = (uint32_t) bits << 16;
    const uint32_t sign   = f & UINT32_C(0x80000000);
    const uint32_t shl1_f = f + f;

    const uint32_t exp_offset = UINT32_C(0xE0) << 23;
    const float    exp_scale  = decode_float(UINT32_C(0x7800000));
    const float    normalized_value
        = decode_float((shl1_f >> 4) + exp_offset) * exp_scale;

    const uint32_t magic_mask = UINT32_C(126) << 23;
    const float    magic_bias = 0.5f;
    const float    denormalized_value
        = decode_float((shl1_f >> 17) | magic_mask) - magic_bias;

    const uint32_t denormalized_cutoff = UINT32_C(1) << 27;
    const uint32_t result
        = sign
          | (shl1_f < denormalized_cutoff ? encode_float(denormalized_value)
                                          : encode_float(normalized_value));
    return decode_float(result);
}

bfloat16_t encode_bfloat16(float value) {
    float_data_t f32;
    f32.value = value;

    // Take the higher 16 bits of the float32 representation
    uint32_t bits = f32.bits;

    // Handle NaN: force to quiet NaN
    if ((bits & 0x7fffffff) > 0x7f800000) {
        return (bits >> 16) | 0x0040;
    }

    // Handle subnormals: flush to zero
    if ((bits & 0x7f800000) == 0) {
        return (bits >> 16) & 0x8000;
    }

    // Rounding: round to nearest even
    uint32_t rounding_bias = (bits & 0x0000ffff) > 0x00007fff    ? 1
                             : (bits & 0x00018000) == 0x00018000 ? 1
                                                                 : 0;
    return (bits + rounding_bias) >> 16;
}

float decode_bfloat16(bfloat16_t bits) {
    // this can probably be simplified since the mapping directly correlates
    // with a 32-bit layout, e.g. the sign bit and exponent are the same and the
    // mantissa is simply expanded as a result. Therefore, it's probably safe to
    // just bit shift back to a 32-bit form. This requires testing to verify.
    uint32_t result = ((uint32_t) bits) << 16;

    // Check for NaN or infinity
    if ((result & 0x7f800000) == 0x7f800000) {
        // NaN or infinity case
        return result & 0x00400000 ? NAN : INFINITY;
    }

    // Subnormal or zero
    if ((result & 0x7f800000) == 0) {
        return result & 0x80000000 ? -0.0f : 0.0f;
    }

    return decode_float32(result);
}

// Convert a 32-bit floating-point number to a 8-bit floating-point number
float16_t encode_float8(float8_t value) {
    // TODO: This is not right, but uses 16-bit conversion as a guide
    float_data_t f32;
    f32.value  = value;
    uint32_t f = f32.bits;

    uint32_t sign     = (f >> 16) & 0x8000;
    uint32_t exponent = ((f >> 23) & 0xFF) - 127 + 15;
    uint32_t mantissa = (f & 0x007FFFFF) >> 13;

    if (exponent <= 0) {
        if (exponent < -10) {
            return 0;
        }
        mantissa   = (f & 0x007FFFFF) | 0x00800000;
        mantissa >>= (1 - exponent);
        return sign | (mantissa >> 13);
    } else if (exponent == 0xFF - (127 - 15)) {
        if (mantissa) {
            return sign | 0x7E00; // NaN
        } else {
            return sign | 0x7C00; // Infinity
        }
    } else if (exponent > 30) {
        return sign | 0x7C00; // Infinity
    }

    return sign | (exponent << 10) | mantissa;
}

// Convert a 16-bit floating-point number to a 8-bit floating-point number
float decode_float8(float8_t bits) {
    // TODO: This is not right, but uses 16-bit conversion as a guide
    uint32_t sign     = (bits & 0x8000) << 16;
    uint32_t exponent = (bits & 0x7C00) >> 10;
    uint32_t mantissa = (bits & 0x03FF) << 13;

    if (exponent == 0) {
        if (mantissa == 0) {
            return sign ? -0.0f : 0.0f;
        }
        while (!(mantissa & 0x00800000)) {
            mantissa <<= 1;
            exponent--;
        }
        exponent++;
        mantissa &= ~0x00800000;
    } else if (exponent == 0x1F) {
        return sign | (mantissa ? 0x7FC00000 : 0x7F800000);
    }

    exponent = exponent + (127 - 15);
    return decode_float32(sign | (exponent << 23) | mantissa);
}
