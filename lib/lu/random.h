
#ifndef LU_RANDOM_H
#define LU_RANDOM_H

#include <stdlib.h>
#include <stdint.h>

#include "log.h"

/**
 * @file
 *
 * Pseudo-random number generators.
 *
 * Because these share a common interface the generator used can be changed
 * by simply changing the constructor.
 *
 * Currently two generators are supported:
 *
 * * splitmix64 - A simple, fast generator with a small range.
 *   See http://xoroshiro.di.unimi.it/splitmix64.c
 *
 * * xoroshiro128+ - A more complex, slightly slower generator with a larger
 *   range.  See http://xoroshiro.di.unimi.it/
 */

/// Internally all generators return a value between 0 and `LURAN_MAX`.
#define LURAN_MAX UINT64_MAX

struct luran;
/// The interface for generating a random `uint64_t` that any implementation must provide.
typedef uint64_t luran_next(struct luran *rand);
/// The interface for freeing the `luran` struct that any implementation must provide.
typedef int luran_free(struct luran **rand, int status);

/// Any random implementation must provide an instance of this struct with appropriate members.
typedef struct luran {
    luran_next *next;  ///< The implementation's generator function.
    luran_free *free;  ///< The implementation's free function.
    void *state;  ///< Opaque state available for use by the implementation.
} luran;

/// Create a `luran` instance with the splitmix64 generator.
int luran_mksplitmix64(lulog *log, luran **rand, uint64_t seed);

/// Create a `luran` instance with the xoroshiro128+ generator.
int luran_mkxoroshiro128plus(lulog *log, luran **rand, uint64_t seed);

// these map (continuously) between [MIN(t),MAX(t)] for the two types
// so that we can generate ranges in unsigned integers and return them as
// signed.

/// Convert from `int64_t` to `uint64_t`.
uint64_t luran_remove_sign(int64_t n);
/// Convert from `uint64_t` to `int64_t`.
int64_t luran_add_sign(uint64_t u);

/// Generate a double value, in the range `[0,1)`.
double luran_double(luran *rand);
/// Generate `uint64_t` value in the range `[lo,hi]`
uint64_t luran_uint64_range(luran *rand, uint64_t lo, uint64_t hi);
/// Generate `int64_t` value in the range `[lo,hi]`
int64_t luran_int64_range(luran *rand, int64_t lo, int64_t hi);

/// Shuffle an array of data using Fisher-Yates.
int luran_shuffle(lulog *log, luran *rand, void *data, size_t size, size_t n);

#endif
