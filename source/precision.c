/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/precision.h
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
bool float_is_close(
    float a, float b, float relative /*= 1e-3f*/, float absolute /*= 0.0f*/
) {
    return fabsf(a - b)
           <= fmaxf(relative * fmaxf(fabsf(a), fabsf(b)), absolute);
}

void* encode_float(float value, data_type_t dtype) {
    float_flex_t flex;
    flex.value.f32 = value;

    switch (dtype) {
        case TYPE_FLOAT_F32:
            return &flex.value.uint32;
        default:
            break;
    }

    return NULL;
}

float decode_float(const float_flex_t* flex) {
    return 0.0f;
}

// Function to encode a float into its IEEE-754 binary32 representation
uint32_t encode_float32(float value) {
    float_data_t f32;
    f32.value = value;
    return f32.bits;
}

// Function to decode an IEEE-754 binary32 representation into a float
float decode_float32(uint32_t bits) {
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
    const float scale_to_inf
        = decode_float(UINT32_C(0x77800000)); // Upper bound
    const float scale_to_zero
        = decode_float(UINT32_C(0x08800000)); // Lower bound

    const float saturated_f = fabsf(value) * scale_to_inf;
    float       base        = saturated_f * scale_to_zero;

    const uint32_t f      = encode_float(value);
    const uint32_t shl1_f = f + f;
    const uint32_t sign   = f & UINT32_C(0x80000000);
    uint32_t       bias   = shl1_f & UINT32_C(0xFF000000);
    if (bias < UINT32_C(0x71000000)) {
        bias = UINT32_C(0x71000000);
    }

    base = decode_float((bias >> 1) + UINT32_C(0x07800000)) + base;
    const uint32_t bits          = encode_float(base);
    const uint32_t exp_bits      = (bits >> 13) & UINT32_C(0x00007C00);
    const uint32_t mantissa_bits = bits & UINT32_C(0x00000FFF);
    const uint32_t nonsign       = exp_bits + mantissa_bits;

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
