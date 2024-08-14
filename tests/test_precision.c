/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_precision.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible. The
 * simpler, the better.
 */

#include "../include/logger.h"
#include "../include/precision.h"

#include <stdio.h>
#include <string.h>

// π is defined as the ratio, c the circumference, and d the diameter
const float  PI        = 3.141592653589793f;
// τ is defined as the ratio, c the circumference, and r the radius
const float  TAU       = PI * 2;
// The number bits to be represented
// NOTE: This is the maximum number of representable bits
const size_t BIT_WIDTH = 32;

void  print_bit_representation(float32_t n, size_t width);
char* get_binary_representation(float32_t n, size_t width);
bool  compare_binary_strings(const char* str1, const char* str2, size_t length);

// Function to convert an unsigned int into its bit representation
//
// void print_bit_representation(float32_t n, size_t width) {
//     printf("%f -> ", decode_float32(n));

//     for (int i = width - 1; i >= 0; i--) {
//         printf("%d", (n >> i) & 1);
//         if (0 == i % 4) {
//             printf(" ");
//         }
//     }
//     printf("\n");
// }
//
// void print_bit_representation(float32_t n, size_t width) {
//     printf("%f -> ", decode_float32(n));

//     uint32_t sign = (n >> 31) & 0x1;
//     printf("%d ", sign & 0x1);

//     uint32_t exponent = (n >> 23) & 0xFF;
//     for (size_t i = 7; i >= 0; i--) {
//         printf("%d", (exponent >> i) & 0x1);

//         if (0 == i % 4) {
//             printf(" ");
//         }
//     }
//     printf(" ");

//     uint32_t mantissa = n & 0x7FFFFF;
//     for (size_t i = 22; i >= 0; i--) {
//         printf("%d", (mantissa >> i) & 0x1);

//         if (0 == i % 4) {
//             printf(" ");
//         }
//     }
//     printf("\n");
// }
//
void print_bit_representation(float32_t n, size_t width) {
    // Print the floating-point value
    printf("%f -> ", decode_float32(n));

    // Extract the sign bit
    uint32_t sign = (n >> 31) & 0x1;
    printf("%d ", sign);

    // Extract the exponent bits (8 bits)
    uint32_t exponent = (n >> 23) & 0xFF;
    for (int i = 7; i >= 0; i--) {
        printf("%d", (exponent >> i) & 0x1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf(" ");

    // Extract the mantissa bits (23 bits)
    uint32_t mantissa = n & 0x7FFFFF;
    for (int i = 22; i >= 0; i--) {
        printf("%d", (mantissa >> i) & 0x1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

// Function to print the binary representation of a 32-bit number and return a
// pointer to the binary string
char* get_binary_representation(float32_t n, size_t width) {
    // Calculate the required length for the binary string
    size_t binary_length = sizeof(char) * width + 1;

    // allocate memory to the string
    char* bit_string = (char*) malloc(sizeof(char) * binary_length);
    if (NULL == bit_string) {
        LOG(&global_logger, LOG_LEVEL_ERROR, "Memory allocation error.\n");
        return NULL;
    }

    // Convert the unsigned integer to a character
    for (int i = width; i >= 0; i--) {
        bit_string[i] = (char) ((n >> i) & 1);
    }

    // Add a null character at the end
    bit_string[binary_length - 1] = *"\0";

    return bit_string;
}

// Function to compare two binary strings
bool compare_binary_strings(const char* str1, const char* str2, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }

    // If we reach here, both strings match
    return true;
}

int main(void) {
    // NULL for const char* file_path
    // the log level can probably be set via a CLI param or config in the
    // future.
    initialize_global_logger(
        LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, "stream", stderr, NULL
    );

    bool result = true;

    float32_t n = encode_float32(10.0f);
    print_bit_representation(n, BIT_WIDTH);

    // char* bin = get_binary_representation(1, 32);
    // fprintf(stderr, "bin: %s\n", bin);

    if (result) {
        printf("All tests passed.\n");
    } else {
        printf("Tests failed. Please review the logs for more information.\n");
    }

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
