
#ifndef LU_MEM_H
#define LU_MEM_H

#include "lu.h"
#include "lulog.h"

#define LU_ALLOC(ptr, n) if (!(ptr = calloc(n, sizeof(*ptr)))) {status = LU_ERR_MEM; goto exit;}
#define LU_ALLOC_TYPE(ptr, n, type) if (!(ptr = calloc(n, sizeof(type)))) {status = LU_ERR_MEM; goto exit;}

typedef struct lumem {
	size_t unit;
	size_t used;
	size_t capacity;
} lumem;

#define LUMEM_ZERO(type) (lumem){ sizeof(type), 0, 0 }

#define LUMEM_MKFREE(name, type)\
int name(lulog *log, type **ptr, lumem *mem, int prev_status) {\
    free(*ptr); *ptr = NULL;\
    *mem = LUMEM_ZERO(type);\
    return prev_status;\
}

// this provides ADDITIONAL (unused) space
#define LUMEM_MKRESERVE(name, type)\
int name(lulog *log, type **ptr, lumem *mem, size_t n) {\
	LU_STATUS\
	if (mem->capacity - mem->used < n) {\
        int required = mem->capacity > 0 ? mem->capacity : 1;\
        while (required - mem->used < n) required *= 2;\
        if (!(*ptr = realloc(*ptr, mem->unit * required))) {\
            luerror(log, "Cannot realloc %zu bytes", mem->unit * required);\
            status = LU_ERR_MEM;\
            goto exit;\
        }\
        mem->capacity = required;\
        memset(*ptr + mem->unit * mem->used, 0, mem->unit * (required - mem->used));\
	}\
	LU_NO_CLEANUP\
}

#endif
