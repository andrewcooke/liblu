
#ifndef LU_ARRAY_MACROS_H
#define LU_ARRAY_MACROS_H

#include "status.h"
#include "dynamic_memory.h"
#include "minmax.h"

/**
 * @file
 *
 * Macros that extend `dynamic_memory.h` to support dynamic arrays that grow
 * in amortized linear time.
 *
 * The main entry point is `LUARY_MKBASE(stem, type, member)` where:
 *
 * * `stem` is used in names.  So the struct typedef for the array is
 *   `luary_stem` and generated functions are `luary_freestem` etc.
 *
 * * `type` is the type of a single array entry.
 *
 * * `member` is the name of the struct member that is the actual array
 *   (at a minimum the struct must also have a `mem` member which is the
 *   `lumem` instance used to manage allocation).
 *
 * The call to `LUARY_MKBASE(stem, type, member)` generates:
 *
 * * `static int stem_free(type **ptr, lumem *mem, int status)`
 *
 *   An internal (static scoped) helper that frees the array and zeroes
 *   the associated `lumem` instance.
 *
 * * `static int stem_reserve(lulog *log, type **ptr, lumem *mem, size_t n)`
 *
 *   An internal (static scoped) helper that reserves space in the array.
 *
 * * `int luary_reservestem(lulog *log, luary_stem *ptr, size_t n)`
 *
 *   A function to ensure that at least `n` unused entries are allocated.
 *
 * * `int luary_mkstem(lulog *log, luary_stem **ptr, size_t n)`
 *
 *   A constructor function that creates a `luary_stem` instance with space
 *   pre-allocated for `n` instances.
 *
 * * int luary_freestem(luary_stem **ptr, int status)`
 *
 *   A function to free the associated data.
 *
 * These routines assume that `luary_stem` is a typedef struct of the form:
 *
 *     typedef struct {
 *       type *member;
 *       lumem mem;
 *     } luary_stem;
 *
 * (this is *not* generated by the macros and may contain additional fields).
 * See, for example, `strings.h`.
 *
 * To use a different naming convention for the generated functions, call the
 * lower level macros separately.
 */

/// (Generate code to) Reserve memory for an array of a given type.
#define LUARY_MKRESERVE(name, type, internal, data)\
int name(lulog *log, type *ptr, size_t n) {\
    return internal(log, &ptr->data, &ptr->mem, n);\
}

/// (Generate code to) Create an array of a given type.
#define LUARY_MKMAKE(name, type, reserve)\
int name(lulog *log, type **ptr, size_t n) {\
    LU_STATUS\
    LU_ALLOC(log, *ptr, 1)\
    LU_CHECK(reserve(log, *ptr, n))\
    LU_NO_CLEANUP\
}

/// (Generate code to) Free an array of a given type.
#define LUARY_MKFREE(name, type, internal, data)\
int name(type **ptr, int status) {\
    if (ptr && *ptr) {\
        status = internal(&(*ptr)->data, &(*ptr)->mem, status);\
        free(*ptr); *ptr = NULL;\
    }\
    return status;\
}

/// (Generate code to) Create, reserve and free an array of a given type.
#define LUARY_MKBASE(stem, type, member)\
static LUMEM_MKFREE(stem##_free, type)\
static LUMEM_MKRESERVE(stem##_reserve, type)\
LUARY_MKRESERVE(luary_reserve##stem, luary_##stem, stem##_reserve, member)\
LUARY_MKMAKE(luary_mk##stem, luary_##stem, luary_reserve##stem)\
LUARY_MKFREE(luary_free##stem, luary_##stem, stem##_free, member)

/// (Generate code to) Dump the contents of an array to the log.
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
    status = lustr_free(&prefix, status);\
    LU_RETURN\
}

#endif
