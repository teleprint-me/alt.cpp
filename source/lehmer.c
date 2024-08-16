/**
 * Copyright © 2024 Austin Berrio
 *
 * @file source/lehmer.c
 *
 * A simple implementation of the lehmer random number generator in pure C
 *
 * Title: Random number generators: good ones are hard to find
 * Paper: https://dl.acm.org/doi/10.1145/63039.63042
 * Source: https://www.cs.wm.edu/~va/software/park/park.html
 *
 * If you get stuck, see the following for more details:
 * https://math.libretexts.org/Bookshelves/Combinatorics_and_Discrete_Mathematics/Discrete_Mathematics_(Levin)
 * - Reference 1.2 Binomial Coefficients for definitions
 * - Reference 5.1 Generating Functions for details
 */

#include "lehmer.h"

#include <stdio.h>
#include <string.h>

// Create and initialize the state with dynamic stream handling
lehmer_state_t* lehmer_create_state(size_t size, int64_t seed) {
    lehmer_state_t* state = (lehmer_state_t*) malloc(sizeof(lehmer_state_t));
    state->seed           = (int64_t*) malloc(sizeof(int64_t) * size);
    state->stream         = 0;
    state->size           = size;
    state->initialized    = false;

    for (size_t i = 0; i < size; i++) {
        state->seed[i]  = (seed > 0 ? seed : ((int64_t) time(NULL))) + i * A256;
        state->seed[i] %= MODULUS; // Ensure seed is within the modulus range
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

void lehmer_set_seed(lehmer_state_t* state, int64_t value) {
    if (value > 0) {
        value = value % MODULUS; // Ensure seed is within the modulus range
    } else {
        value = ((uint64_t) time(NULL)) % MODULUS; // Use current time as seed
    }
    state->seed[state->stream] = value;
}

int64_t lehmer_get_seed(lehmer_state_t* state) {
    return state->seed[state->stream];
}

void lehmer_select_stream(lehmer_state_t* state, size_t stream) {
    // Explicitly typecast stream to size_t (unsigned long)
    state->stream = (size_t) (stream % STREAMS);
    // Protect against uninitialized streams
    if ((state->initialized == false) && (state->stream != 0)) {
        // NOTE: Circular references are a red flag!
        lehmer_seed_streams(state, DEFAULT); // This is not okay.
    }
}

void lehmer_seed_streams(lehmer_state_t* state, int64_t value) {
    const int64_t quotient      = MODULUS / A256;
    const int64_t remainder     = MODULUS % A256;
    const size_t  stream_backup = state->stream;

    lehmer_select_stream(state, 0); // NOTE: Circular references are a red flag!
    lehmer_set_seed(state, value);  // This is okay.

    state->stream = stream_backup;

    // Initialize seeds for the remaining streams
    for (size_t i = 1; i < state->size; i++) { // Changed STREAMS to state->size
        int64_t new_seed = A256 * (state->seed[i - 1] % quotient)
                           - remainder * (state->seed[i - 1] / quotient);

        if (new_seed > 0) {
            state->seed[i] = new_seed;
        } else {
            state->seed[i] = new_seed + MODULUS;
        }
    }

    state->initialized = true;
}

// Generate the next random number
double lehmer_generate(lehmer_state_t* state) {
    const int64_t quotient  = MODULUS / MULTIPLIER;
    const int64_t remainder = MODULUS % MULTIPLIER;
    int64_t       new_seed;

    new_seed = MULTIPLIER * (state->seed[state->stream] % quotient)
               - remainder * (state->seed[state->stream] / quotient);

    if (new_seed > 0) {
        state->seed[state->stream] = new_seed;
    } else {
        state->seed[state->stream] = new_seed + MODULUS;
    }

    return ((double) state->seed[state->stream] / MODULUS);
}
