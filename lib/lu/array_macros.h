
#ifndef LU_ARRAY_MACROS_H
#define LU_ARRAY_MACROS_H

#include "dynamic_memory.h"


// additional macros that build on lumem.h to define the base of a
// dynamic array


#define LUARRAY_MKRESERVE(name, type, internal, data)\
int name(lulog *log, type *ptr, size_t n) {\
    return internal(log, &ptr->data, &ptr->mem, n);\
}

#define LUARRAY_MKMAKE(name, type, reserve)\
int name(lulog *log, type **ptr, size_t n) {\
    LU_STATUS\
    LU_ALLOC(log, *ptr, 1)\
    LU_CHECK(reserve(log, *ptr, n))\
    LU_NO_CLEANUP\
}

#define LUARRAY_MKFREE(name, type, internal, data)\
int name(type **ptr, int prev_status) {\
    int status = internal(&(*ptr)->data, &(*ptr)->mem, prev_status);\
    free(*ptr); *ptr = NULL;\
    return status;\
}

#define LUARRAY_MKBASE(stem, atype, dtype, data)\
static LUMEM_MKFREE(stem##_free, dtype)\
static LUMEM_MKRESERVE(stem##_reserve, dtype)\
LUARRAY_MKRESERVE(luarray_reserve##stem, atype, stem##_reserve, data)\
LUARRAY_MKMAKE(luarray_mk##stem##n, atype, luarray_reserve##stem)\
LUARRAY_MKFREE(luarray_free##stem, atype, stem##_free, data)

#endif
