
#ifndef LU_arrays_H
#define LU_arrays_H

#include "strings.h"
#include "internal.h"
#include "memory.h"
#include "minmax.h"

/**
 * @file
 *
 * @brief Macros that extend `memory.h` to support dynamic arrays that grow
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
 * * `static int lumem_stem_free(type **ptr, lumem *mem, int status)`
 *
 *   An internal (static scoped) helper that frees the array and zeroes
 *   the associated `lumem` instance.
 *
 * * `static int lumem_stem_reserve(lulog *log, type **ptr, lumem *mem, size_t n)`
 *
 *   An internal (static scoped) helper that reserves space in the array.
 *
 * * `int luary_stem_res(lulog *log, luary_stem *ptr, size_t n)`
 *
 *   A function to ensure that at least `n` unused entries are allocated.
 *
 * * `int luary_stem_mk(lulog *log, luary_stem **ptr, size_t n)`
 *
 *   A constructor function that creates a `luary_stem` instance with space
 *   pre-allocated for `n` instances.
 *
 * * `int luary_stem_free(luary_stem **ptr, int status)`
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
    int status = LU_OK;\
    LU_ALLOC(log, *ptr, 1)\
    try(reserve(log, *ptr, n))\
    finally:\
	return status;\
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
static LUMEM_MKFREE(lumem_##stem##_free, type)\
static LUMEM_MKRESERVE(lumem_##stem##_res, type)\
LUARY_MKRESERVE(luary_##stem##_res, luary_##stem, lumem_##stem##_res, member)\
LUARY_MKMAKE(luary_##stem##_mk, luary_##stem, luary_##stem##_res)\
LUARY_MKFREE(luary_##stem##_free, luary_##stem, lumem_##stem##_free, member)

/// (Generate code to) Dump the contents of an array to the log.
#define LUARY_MKDUMP(name, type, format, ...)\
int name(lulog *log, type *ptr, const char *title, size_t n) {\
    int status = LU_OK;\
    size_t line = 0, nused = min(n, ptr->mem.used);\
    lustr lines = {}, word = {}, prefix = {};\
    try(lustr_mk(log, &lines));\
    try(lustr_mk(log, &word));\
    try(lustr_mk(log, &prefix));\
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
    finally:\
    status = lustr_free(&lines, status);\
    status = lustr_free(&word, status);\
    status = lustr_free(&prefix, status);\
    return status;\
}

#endif
