
#ifndef LU_ARRAY_MACROS_H
#define LU_ARRAY_MACROS_H

#include "dynamic_memory.h"
#include "minmax.h"


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
int name(type **ptr, int status) {\
    if (ptr && *ptr) {\
        status = internal(&(*ptr)->data, &(*ptr)->mem, status);\
        free(*ptr); *ptr = NULL;\
    }\
    return status;\
}

#define LUARRAY_MKSIZE(name, type, data)\
size_t name(type *ptr) {\
    return ptr->mem.used * sizeof(*ptr->data);\
}

#define LUARRAY_MKBASE(stem, atype, dtype, data)\
static LUMEM_MKFREE(stem##_free, dtype)\
static LUMEM_MKRESERVE(stem##_reserve, dtype)\
LUARRAY_MKRESERVE(luarray_reserve##stem, atype, stem##_reserve, data)\
LUARRAY_MKMAKE(luarray_mk##stem##n, atype, luarray_reserve##stem)\
LUARRAY_MKFREE(luarray_free##stem, atype, stem##_free, data)\
LUARRAY_MKSIZE(luarray_size##stem, atype, data)

#define LUARRAY_MKDUMP(name, type, nrow, format, ...)\
int name(lulog *log, type *ptr, const char *title, size_t nlines) {\
    LU_STATUS\
    size_t n = min(nrow * nlines, ptr->mem.used);\
    ludebug(log, "%s - first %zu of %zu (%zu) elements:",\
            title, n, ptr->mem.used, ptr->mem.capacity);\
    lustr line = {}; LU_CHECK(lustr_mk(log, &line));\
    for (size_t i = 0; i < n; ++i) {\
        lustr_appendf(log, &line, format, __VA_ARGS__);\
        if (!((i+1) % nrow) || i == n-1) {\
            ludebug(log, "%s", line.c);\
            LU_CHECK(lustr_printf(log, &line, "%zu: ", i+1));\
        } else {\
            lustr_append(log, &line, ", ");\
        }\
    }\
LU_CLEANUP\
    status = lustr_free(&line, status);\
    LU_RETURN\
}

#endif
