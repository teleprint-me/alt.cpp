/**
 * @file include/lehmer.h
 */

#ifndef LEHMER_H
#define LEHMER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MODULUS    2147483647 // Mersenne prime number used as modulus (2^31 - 1)
#define MULTIPLIER 48271      // Multiplier for the Lehmer RNG
#define CHECK      399268537  // Check value for correctness
#define STREAMS    256        // Number of streams
#define A256       22925      // Jump multiplier for stream separation
#define DEFAULT    123456789  // Default seed value

// if we can only use positive values, why even bother with negative values?
// doesn't make sense to me. might be better to enforce unsigned values,
// e.g. use something like uint64_t isntead.
typedef struct LehmerState {
    int64_t* seed;        // Current state of each stream
    size_t   stream;      // Current stream index, defaults to 0
    size_t   size;        // Dynamic number of streams
    bool     initialized; // true if RNG is initialized
} lehmer_state_t;

lehmer_state_t* lehmer_create_state(size_t size, int64_t seed);
void            lehmer_free_state(lehmer_state_t* state);

void    lehmer_set_seed(lehmer_state_t* state, int64_t value);
int64_t lehmer_get_seed(lehmer_state_t* state);

void lehmer_select_stream(lehmer_state_t* state, size_t stream);
void lehmer_seed_streams(lehmer_state_t* state, int64_t value);

float lehmer_generate(lehmer_state_t* state);

#endif // LEHMER_H
