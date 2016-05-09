
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "lu.h"
#include "lumem.h"
#include "lustatus.h"
#include "lurand.h"


static int generic_free(lurand **rand, int prev_status) {
    LU_STATUS
    if (*rand) free((*rand)->state);
    free(*rand);
    *rand = NULL;
    LU_NO_CLEANUP2(prev_status)
}


typedef struct splitmix64_state {
    uint64_t x;
} splitmix64_state;

static uint64_t splitmix64_next(lurand *rand) {
    splitmix64_state *state = (splitmix64_state*)rand->state;
    uint64_t z = (state->x += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

int lurand_mksplitmix64(lulog *log, lurand **rand, uint64_t seed) {
    LU_STATUS
    LU_ALLOC(log, *rand, 1)
    LU_ALLOC_TYPE(log, (*rand)->state, 1, splitmix64_state);
    splitmix64_state *state = (splitmix64_state*)(*rand)->state;
    state->x = seed;
    (*rand)->next = splitmix64_next;
    (*rand)->free = generic_free;
    ludebug(log, "Allocated splitmix64 with seed %" PRIu64, seed);
    LU_NO_CLEANUP
}


// code from http://xoroshiro.di.unimi.it/xoroshiro128plus.c
// (David Blackman and Sebastiano Vigna (vigna@acm.org), public domain)
// modified to give [0..max] range (subtracting 1)

typedef struct xoroshiro128plus_state {
    uint64_t s0;
    uint64_t s1;
} xoroshiro128plus_state;

static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static uint64_t xoroshiro128plus_next(lurand *rand) {
    xoroshiro128plus_state *state = (xoroshiro128plus_state*)rand->state;
    const uint64_t s0 = state->s0;
    uint64_t s1 = state->s1;
    const uint64_t result = s0 + s1;
    s1 ^= s0;
    state->s0 = rotl(s0, 55) ^ s1 ^ (s1 << 14);
    state->s1 = rotl(s1, 36);
    return result;
}

static int xoroshiro128plus_free(lurand **rand, int prev_status) {
    LU_STATUS
    if (*rand) free((*rand)->state);
    free(*rand);
    *rand = NULL;
    LU_NO_CLEANUP2(prev_status)
}

// seeding is via splitmix64, as recommended at
// http://xoroshiro.di.unimi.it/
int lurand_mkxoroshiro128plus(lulog *log, lurand **rand, uint64_t seed) {
    LU_STATUS
    lurand *srand = NULL;
    LU_CHECK(lurand_mksplitmix64(log, &srand, seed));
    uint64_t seed0 = 0, seed1 = 0;
    while (seed0 == 0) seed0 = srand->next(srand);
    while (seed1 == 0) seed1 = srand->next(srand);
    LU_ALLOC(log, *rand, 1)
    LU_ALLOC_TYPE(log, (*rand)->state, 1, xoroshiro128plus_state);
    xoroshiro128plus_state *state = (xoroshiro128plus_state*)(*rand)->state;
    state->s0 = seed0;
    state->s1 = seed1;
    (*rand)->next = xoroshiro128plus_next;
    (*rand)->free = xoroshiro128plus_free;
    ludebug(log, "Allocated xoroshiro128plus with seeds %" PRIu64
            " and %" PRIu64, seed0, seed1);
LU_CLEANUP
    if (srand) status = srand->free(&srand, status);
    LU_RETURN
}


// http://xoroshiro.di.unimi.it/
double lurand_double(lurand *rand) {
    const union {uint64_t i; double d;} u = {.i = UINT64_C(0x3FF) << 52 | rand->next(rand) >> 12 };
    return u.d - 1.0;
}

// http://stackoverflow.com/a/17554531
uint64_t lurand_uint64_range(lurand *rand, uint64_t lo, uint64_t hi) {
    uint64_t tmp, range, buckets, limit;
    if (lo > hi) {
        tmp = hi; hi = lo; lo = tmp;
    } else if (lo == hi) {
        return lo;
    }
    uint64_t value = rand->next(rand);
    if (lo == 0 && hi == LURAND_MAX) {
        return value;
    }
    range = hi - lo + 1;
    buckets = LURAND_MAX / range;
    limit = buckets * range;
    while (value >= limit) value = rand->next(rand);
    return lo + value / buckets;
}


// http://stackoverflow.com/a/31203972
uint64_t lurand_remove_sign(int64_t n) {
    uint64_t u = UINT64_C(1) + INT64_MAX;
    u += n;
    return u;
}

int64_t lurand_add_sign(uint64_t u) {
    uint64_t zero = lurand_remove_sign(0);
    if (u >= zero) {
        return u - zero;
    } else {
        int64_t n1 = INT64_MIN, n2 = u;
        return n1 + n2;
    }
}

int64_t lurand_int64_range(lurand *rand, int64_t lo, int64_t hi) {
    int64_t tmp;
    if (lo > hi) {
        tmp = hi; hi = lo; lo = tmp;
    } else if (lo == hi) {
        return lo;
    }
}

