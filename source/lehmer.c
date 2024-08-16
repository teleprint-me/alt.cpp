/**
 * @file source/lehmer.c
 *
 * A simple implementation of the lehmer random number generator in pure C
 */

#include "lehmer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Create and initialize the state with dynamic stream handling
lehmer_state_t* lehmer_create_state(size_t stream_count, int64_t initial_seed) {
    lehmer_state_t* state = (lehmer_state_t*) malloc(sizeof(lehmer_state_t));
    state->seed           = (int64_t*) malloc(sizeof(int64_t) * stream_count);
    state->stream_count   = stream_count;
    state->stream_index   = 0;
    state->initialized    = false; // Set to 1 after having set planted seeds

    for (size_t i = 0; i < stream_count; i++) {
        state->seed[i]
            = initial_seed; // Initialize all streams with the same seed
    }

    return state;
}

// Free the allocated memory
void lehmer_free_state(lehmer_state_t* state) {
    if (state) {
        if (state->seed) {
            free(state->seed);
        }
        free(state);
    }
}

// Generate the next random number
double lehmer_generate(lehmer_state_t* state) {
    const int64_t quotient  = MODULUS / MULTIPLIER;
    const int64_t remainder = MODULUS % MULTIPLIER;
    int64_t       new_seed;

    new_seed = MULTIPLIER * (state->seed[state->stream_index] % quotient)
               - remainder * (state->seed[state->stream_index] / quotient);

    if (new_seed > 0) {
        state->seed[state->stream_index] = new_seed;
    } else {
        state->seed[state->stream_index] = new_seed + MODULUS;
    }

    return ((double) state->seed[state->stream_index] / MODULUS);
}

void lehmer_seed(lehmer_state_t* state, int64_t initial_seed) {
    return;
}

int main(void) {
    size_t          stream_count = 256;
    int64_t         initial_seed = DEFAULT;
    lehmer_state_t* state = lehmer_create_state(stream_count, initial_seed);

    double random = lehmer_generate(state);
    printf("PRN: %f\n", random);

    lehmer_free_state(state);
    return 0;
}
