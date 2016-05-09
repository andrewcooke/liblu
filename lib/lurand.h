
#ifndef LU_RAND_H
#define LU_RAND_H

#include <stdlib.h>
#include <stdint.h>

#include "lulog.h"

#define LURAND_MAX UINT64_MAX

struct lurand;

typedef uint64_t lurand_next(struct lurand *rand);
typedef int lurand_free(struct lurand **rand, int status);

typedef struct lurand {
    lurand_next *next;
    lurand_free *free;
    void *state;
} lurand;

// short period - used internally for seeding
int lurand_mksplitmix64(lulog *log, lurand **rand, uint64_t seed);

// longer period (s^128-1).  has low probability of zero.
int lurand_mkxoroshiro128plus(lulog *log, lurand **rand, uint64_t seed);

uint64_t lurand_remove_sign(int64_t n);
int64_t lurand_add_sign(uint64_t u);

double lurand_double(lurand *rand);
uint64_t lurand_uint64_range(lurand *rand, uint64_t lo, uint64_t hi);
int64_t lurand_int64_range(lurand *rand, int64_t lo, int64_t hi);

int lurand_shuffle(lulog *log, lurand *rand, void *data, size_t size, size_t n);

#endif
