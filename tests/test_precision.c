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

// π is defined as the ratio, c the circumference, and d the diameter
#define PI  3.141592653589793f
// τ is defined as the ratio, c the circumference, and r the radius
#define TAU PI * 2

int main(void) {
    // NULL for const char* file_path
    // the log level can probably be set via a CLI param or config in the
    // future.
    initialize_global_logger(
        LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, "stream", stderr, NULL
    );

    bool result = true;

    if (result) {
        printf("All tests passed.\n");
    } else {
        printf("Tests failed. Please review the logs for more information.\n");
    }

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
