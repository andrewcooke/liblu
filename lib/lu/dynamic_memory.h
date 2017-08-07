
#ifndef LU_DYNAMIC_MEMORY_H
#define LU_DYNAMIC_MEMORY_H

#include <stdlib.h>

#include "status_codes.h"

/**
 * @file
 *
 * Support for dynamically allocating memory.  In addition to various
 * helper macros, the `lumem` structure and `LUMEM_MKRESERVE()` macro
 * support arrays that grow dynamically in amortized linear time (by
 * doubling allocated size as needed).
 *
 * For higher level functionality, see `array_macros.h` and `strings.h`.
 */

/// (Generate code to) Allocate (zeroed) memory matching the type of `ptr`.
/** This assumes that the conventions in `status.h` are followed. */
#define LU_ALLOC(log, ptr, n)\
if (!(ptr = calloc(n, sizeof(*ptr)))) {\
    luerror(log, "Cannot allocate %zu bytes", n * sizeof(*ptr));\
    status = LU_ERR_MEM; goto exit;\
}

/// (Generate code to) Allocate (zeroed) memory for the given type.
/** This assumes that the conventions in `status.h` are followed. */
#define LU_ALLOC_TYPE(log, ptr, n, type)\
if (!(ptr = calloc(n, sizeof(type)))) {\
    luerror(log, "Cannot allocate %zu bytes", n * sizeof(type));\
    status = LU_ERR_MEM; goto exit;\
}

/// (Generate code to) Allocate the given amount of (zeroed) memory.
/** This assumes that the conventions in `status.h` are followed. */
#define LU_ALLOC_SIZE(log, ptr, size)\
if (!(ptr = calloc(1, size))) {\
    luerror(log, "Cannot allocate %zu bytes", size);\
    status = LU_ERR_MEM; goto exit;\
}

/// Data required for dynamic memory management.
typedef struct lumem {
	size_t used;  ///< The amount of memory currently in use.
	size_t capacity;  ///< The total amount of memory allocated.
} lumem;

/// An instance of `lumem` with no memory allocated or used.
#define LUMEM_ZERO (lumem){0, 0}

/// (Generate code to) Free memory for a given type.
/** This frees the array and zeroes the associated `lumem` instance. */
#define LUMEM_MKFREE(name, type)\
int name(type **ptr, lumem *mem, int prev_status) {\
    free(*ptr); *ptr = NULL;\
    *mem = LUMEM_ZERO;\
    return prev_status;\
}

/// (Generate code to) Reserve memory for a given type.
/** This ensures that memory is available for an additional  `n` instances
 * of `*ptr`. */
#define LUMEM_MKRESERVE(name, type)\
int name(struct lulog *log, type **ptr, lumem *mem, size_t n) {\
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
