
#ifndef LU_DYNAMIC_MEMORY_H
#define LU_DYNAMIC_MEMORY_H

#include <stdlib.h>

#include "error_codes.h"


// basic support for dynamically allocated memory.  see luarray_mem.h for
// use in constructing dynamic arrays.

typedef struct lulog lulog;  // avoid dependency loops

#define LU_ALLOC(log, ptr, n)\
if (!(ptr = calloc(n, sizeof(*ptr)))) {\
    luerror(log, "Cannot allocate %zu bytes", n * sizeof(*ptr));\
    status = LU_ERR_MEM; goto exit;\
}
#define LU_ALLOC_TYPE(log, ptr, n, type)\
if (!(ptr = calloc(n, sizeof(type)))) {\
    luerror(log, "Cannot allocate %zu bytes", n * sizeof(type));\
    status = LU_ERR_MEM; goto exit;\
}

typedef struct lumem {
	size_t used;
	size_t capacity;
} lumem;

#define LUMEM_ZERO (lumem){0, 0}

#define LUMEM_MKFREE(name, type)\
int name(type **ptr, lumem *mem, int prev_status) {\
    free(*ptr); *ptr = NULL;\
    *mem = LUMEM_ZERO;\
    return prev_status;\
}

// this provides ADDITIONAL (unused) space
#define LUMEM_MKRESERVE(name, type)\
int name(lulog *log, type **ptr, lumem *mem, size_t n) {\
	LU_STATUS\
	if (mem->capacity - mem->used < n) {\
        int required = mem->capacity > 0 ? mem->capacity : 1;\
        size_t unit = sizeof(type);\
        while (required - mem->used < n) required *= 2;\
        if (!(*ptr = realloc(*ptr, unit * required))) {\
            luerror(log, "Cannot reallocate %zu bytes", unit * required);\
            status = LU_ERR_MEM;\
            goto exit;\
        }\
        mem->capacity = required;\
        /* no need for unit on left as ptr typed!!! */\
        memset(*ptr + mem->used, 0, unit * (required - mem->used));\
	}\
	LU_NO_CLEANUP\
}

#endif