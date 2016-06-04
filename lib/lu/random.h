
#ifndef LU_RANDOM_H
#define LU_RANDOM_H

#include <stdlib.h>
#include <stdint.h>

#include "log.h"

#define LURAN_MAX UINT64_MAX

struct luran;

typedef uint64_t luran_next(struct luran *rand);
typedef int luran_free(struct luran **rand, int status);

typedef struct luran {
    luran_next *next;
    luran_free *free;
    void *state;
} luran;

// short period - used internally for seeding
int luran_mksplitmix64(lulog *log, luran **rand, uint64_t seed);

// longer period (s^128-1).  has low probability of zero.
int luran_mkxoroshiro128plus(lulog *log, luran **rand, uint64_t seed);

// these map (continuously) between [MIN(t),MAX(t)] for the two types
// so that we can generate ranges in unsigned integers and return them as
// signed.
uint64_t luran_remove_sign(int64_t n);
int64_t luran_add_sign(uint64_t u);

double luran_double(luran *rand);
uint64_t luran_uint64_range(luran *rand, uint64_t lo, uint64_t hi);
int64_t luran_int64_range(luran *rand, int64_t lo, int64_t hi);

int luran_shuffle(lulog *log, luran *rand, void *data, size_t size, size_t n);

#endif
