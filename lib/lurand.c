
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "lu.h"
#include "lumem.h"
#include "lustatus.h"
#include "lurand.h"


typedef struct xoroshiro128plus_state {
    uint64_t s0;
    uint64_t s1;
} xoroshiro128plus_state;

// code from http://xoroshiro.di.unimi.it/xoroshiro128plus.c
// (David Blackman and Sebastiano Vigna (vigna@acm.org), public domain)

static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static uint64_t xoroshiro128plus_next(lurand *lrand) {
    xoroshiro128plus_state *state = (xoroshiro128plus_state*)lrand->state;
    const uint64_t s0 = state->s0;
    uint64_t s1 = state->s1;
    const uint64_t result = s0 + s1;
    s1 ^= s0;
    state->s0 = rotl(s0, 55) ^ s1 ^ (s1 << 14);
    state->s1 = rotl(s1, 36);
    return result;
}

static int xoroshiro128plus_free(lurand **lrand, int prev_status) {
    LU_STATUS
    if (*lrand) free((*lrand)->state);
    free(*lrand);
    *lrand = NULL;
    LU_NO_CLEANUP2(prev_status)
}

// note that simple seeds will give a simple initial value - there is no
// warm-up included here
int lurand_mkxoroshiro128plus(lulog *log, lurand **lrand, uint64_t seed0, uint64_t seed1) {
    LU_STATUS
    LU_ASSERT(seed0 != 0 || seed1 != 0, "Cannot use zero seed for xoroshiro128plus", LU_ERR_ARG);
    LU_ALLOC(log, *lrand, 1)
    LU_ALLOC_TYPE(log, (*lrand)->state, 1, xoroshiro128plus_state);
    xoroshiro128plus_state *state = (xoroshiro128plus_state*)(*lrand)->state;
    state->s0 = seed0;
    state->s1 = seed1;
    (*lrand)->next = xoroshiro128plus_next;
    (*lrand)->free = xoroshiro128plus_free;
    ludebug(log, "Allocated xoroshiro128plus with seeds %" PRId64
            " and %" PRId64, seed0, seed1);
    LU_NO_CLEANUP
}

int lurand_mkxoroshiro128plusauto(lulog *log, lurand **lrand) {
    while (1) {
        uint64_t s0 = rand();
        uint64_t s1 = rand();
        if (s0 | s1) return lurand_mkxoroshiro128plus(log, lrand, s0, s1);
    }
}
