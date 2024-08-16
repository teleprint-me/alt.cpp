/**
 * @file include/lehmer.h
 */

#ifndef LEHMER_H
#define LEHMER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MODULUS    2147483647 // Large prime number used as modulus (2^31 - 1)
#define MULTIPLIER 48271      // Multiplier for the Lehmer RNG
#define CHECK      399268537  // Check value for correctness
#define STREAMS    256        // Number of streams
#define A256       22925      // Jump multiplier for stream separation
#define DEFAULT    123456789  // Default seed value

typedef struct LehmerState {
    int64_t* seed;
    size_t   stream_index;
    size_t   stream_count; // Dynamic number of streams
    bool     initialized;
} lehmer_state_t;

lehmer_state_t* lehmer_create_state(size_t stream_count, int64_t initial_seed);
void            lehmer_free_state(lehmer_state_t* state);

float lehmer_generate(lehmer_state_t* state);

#endif // LEHMER_H
