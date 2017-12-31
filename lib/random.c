
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <lu/status.h>

#include "lu/internal.h"
#include "lu/memory.h"
#include "lu/random.h"


static int generic_free(luran **rand, int prev_status) {
	int status = LU_OK;
	if (*rand) free((*rand)->state);
	free(*rand);
	*rand = NULL;
	return status;
}

// code from http://xoroshiro.di.unimi.it/splitmix64.c
// (Sebastiano Vigna (vigna@acm.org), public domain)

typedef struct splitmix64_state {
	uint64_t x;
} splitmix64_state;

static uint64_t splitmix64_next(luran *rand) {
	splitmix64_state *state = (splitmix64_state*)rand->state;
	uint64_t z = (state->x += UINT64_C(0x9E3779B97F4A7C15));
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);
}

int luran_mksplitmix64(lulog *log, luran **rand, uint64_t seed) {
	int status = LU_OK;
	LU_ALLOC(log, *rand, 1)
	LU_ALLOC_TYPE(log, (*rand)->state, 1, splitmix64_state);
	splitmix64_state *state = (splitmix64_state*)(*rand)->state;
	state->x = seed;
	(*rand)->next = splitmix64_next;
	(*rand)->free = generic_free;
	ludebug(log, "Allocated splitmix64 with seed %" PRIu64, seed);
	finally:
	return status;
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

static uint64_t xoroshiro128plus_next(luran *rand) {
	xoroshiro128plus_state *state = (xoroshiro128plus_state*)rand->state;
	const uint64_t s0 = state->s0;
	uint64_t s1 = state->s1;
	const uint64_t result = s0 + s1;
	s1 ^= s0;
	state->s0 = rotl(s0, 55) ^ s1 ^ (s1 << 14);
	state->s1 = rotl(s1, 36);
	return result;
}

static int xoroshiro128plus_free(luran **rand, int prev_status) {
	int status = LU_OK;
	if (*rand) free((*rand)->state);
	free(*rand);
	*rand = NULL;
	return status;
}

// seeding is via splitmix64, as recommended at
// http://xoroshiro.di.unimi.it/
int luran_mkxoroshiro128plus(lulog *log, luran **rand, uint64_t seed) {
	int status = LU_OK;
	luran *srand = NULL;
	try(luran_mksplitmix64(log, &srand, seed));
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
	ludebug(log, "Allocated xoroshiro128plus with seeds %" PRIu64 " and %" PRIu64, seed0, seed1);
	finally:
	if (srand) status = srand->free(&srand, status);
	return status;
}


// http://xoroshiro.di.unimi.it/
double luran_double(luran *rand) {
	const union {uint64_t i; double d;} u = {.i = UINT64_C(0x3FF) << 52 | rand->next(rand) >> 12 };
	return u.d - 1.0;
}

// http://stackoverflow.com/a/17554531
uint64_t luran_uint64_range(luran *rand, uint64_t lo, uint64_t hi) {
	uint64_t tmp;
	if (lo > hi) {
		tmp = hi; hi = lo; lo = tmp;
	} else if (lo == hi) {
		return lo;
	}
	uint64_t value = rand->next(rand);
	if (lo == 0 && hi == LURAN_MAX) {
		return value;
	} else {
		uint64_t range = hi - lo + 1;
		uint64_t buckets = LURAN_MAX / range;
		uint64_t limit = buckets * range;
		while (value >= limit) value = rand->next(rand);
		return lo + value / buckets;
	}
}


// http://stackoverflow.com/a/31203972
uint64_t luran_remove_sign(int64_t n) {
	uint64_t u = UINT64_C(1) + INT64_MAX;
	u += n;
	return u;
}

int64_t luran_add_sign(uint64_t u) {
	if (u <= INT64_MAX) {
		int64_t n1 = INT64_MIN, n2 = u;
		return n1 + n2;
	} else {
		uint64_t zero = luran_remove_sign(0);
		int64_t n = u - zero;
		return n;
	}
}

int64_t luran_int64_range(luran *rand, int64_t lo, int64_t hi) {
	return luran_add_sign(
			luran_uint64_range(rand,
					luran_remove_sign(lo), luran_remove_sign(hi)));
}


// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
int luran_shuffle(lulog *log, luran *rand, void *data, size_t size, size_t n) {
	int status = LU_OK;
	uint64_t i, j, k;
	char *cdata = (char*)data;
	for (i = n-1; i > 0; --i) {
		j = luran_uint64_range(rand, 0, i);
		for (k = 0; k < size; ++k) {
			char tmp = cdata[i*size+k];
			cdata[i*size+k] = cdata[j*size+k];
			cdata[j*size+k] = tmp;
		}
	}
	finally:
	return status;
}
