
#ifndef LU_ARRAY_MACROS_H
#define LU_ARRAY_MACROS_H

#include "dynamic_memory.h"
#include "minmax.h"


// additional macros that build on lumem.h to define the base of a
// dynamic array


#define LUARY_MKRESERVE(name, type, internal, data)\
int name(lulog *log, type *ptr, size_t n) {\
    return internal(log, &ptr->data, &ptr->mem, n);\
}

#define LUARY_MKMAKE(name, type, reserve)\
int name(lulog *log, type **ptr, size_t n) {\
    LU_STATUS\
    LU_ALLOC(log, *ptr, 1)\
    LU_CHECK(reserve(log, *ptr, n))\
    LU_NO_CLEANUP\
}

#define LUARY_MKFREE(name, type, internal, data)\
int name(type **ptr, int status) {\
    if (ptr && *ptr) {\
        status = internal(&(*ptr)->data, &(*ptr)->mem, status);\
        free(*ptr); *ptr = NULL;\
    }\
    return status;\
}

#define LUARY_MKSIZE(name, type, data)\
size_t name(type *ptr) {\
    return ptr->mem.used * sizeof(*ptr->data);\
}

#define LUARY_MKBASE(stem, atype, dtype, data)\
static LUMEM_MKFREE(stem##_free, dtype)\
static LUMEM_MKRESERVE(stem##_reserve, dtype)\
LUARY_MKRESERVE(luary_reserve##stem, atype, stem##_reserve, data)\
LUARY_MKMAKE(luary_mk##stem##n, atype, luary_reserve##stem)\
LUARY_MKFREE(luary_free##stem, atype, stem##_free, data)\
LUARY_MKSIZE(luary_size##stem, atype, data)

#define LUARY_MKDUMP(name, type, format, ...)\
int name(lulog *log, type *ptr, const char *title, size_t n) {\
    LU_STATUS\
    size_t line = 0, nused = min(n, ptr->mem.used);\
    lustr lines = {}, word = {}, prefix = {};\
    LU_CHECK(lustr_mk(log, &lines))\
    LU_CHECK(lustr_mk(log, &word))\
    LU_CHECK(lustr_mk(log, &prefix))\
    ludebug(log, "%s - first %zu of %zu (%zu) elements:",\
            title, nused, ptr->mem.used, ptr->mem.capacity);\
    for (size_t i = 0; i < n; ++i) {\
        lustr_printf(log, &word, format, __VA_ARGS__);\
        if (line > 0 && line + word.mem.used > 77) {\
            lustr_printf(log, &prefix, "%d: ", i);\
            lustr_appendf(log, &lines, "\n%s%s", prefix.c, word.c);\
            line = prefix.mem.used + word.mem.used - 2;\
        } else if (line > 0){\
            lustr_appendf(log, &lines, ", %s", word.c);\
            line += 1 + word.mem.used;\
        } else {\
            lustr_appendf(log, &lines, "0: %s", word.c);\
            line += 2 + word.mem.used;\
        }\
    }\
    lulog_lines(log, lulog_level_debug, lines.c);\
LU_CLEANUP\
    status = lustr_free(&lines, status);\
    status = lustr_free(&word, status);\
    LU_RETURN\
}

#endif
