
#ifndef LU_MEM_H
#define LU_MEM_H

#include "lu.h"
#include "lulog.h"

// TODO - add logging, dogfood
#define LU_ALLOC(ptr, n) if (!(ptr = calloc(n, sizeof(*ptr)))) {status = LU_ERR_MEM; goto exit;}
#define LU_ALLOC_TYPE(ptr, n, type) if (!(ptr = calloc(n, sizeof(type)))) {status = LU_ERR_MEM; goto exit;}

typedef struct lumem {
	size_t unit;
	size_t used;
	size_t available;
} lumem;

#define LU_MKMEM(type) (lumem){ sizeof(type), 0, 0 }

int lumem_reset(lulog *log, void **ptr, lumem *mem);
int lumem_reserve(lulog *log, void **ptr, lumem *mem, size_t n);

#endif
