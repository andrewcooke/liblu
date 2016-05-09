
#ifndef LU_RAND_H
#define LU_RAND_H

#include <stdint.h>

#include "lulog.h"

struct lurand;

typedef uint64_t lurand_next(struct lurand *rand);
typedef int lurand_free(struct lurand **rand, int status);

typedef struct lurand {
    lurand_next *next;
    lurand_free *free;
    void *state;
} lurand;

int lurand_mkxoroshiro128plus(lulog *log, lurand **rand, uint64_t seed0, uint64_t seed1);
int lurand_mkxoroshiro128plusauto(lulog *log, lurand **rand);

#endif
