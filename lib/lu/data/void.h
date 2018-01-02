
#ifndef LU_DATA_VOID_H
#define LU_DATA_VOID_H

#include "../log.h"
#include "../memory.h"

/**
 * @file
 *
 * @brief Support for arrays of void* values.
 */
/// @{

/// An array of void* values.
typedef struct luary_void {
    void **ptr;
    lumem mem;
} luary_void;

/// Create an array of void* values.
int luary_void_mk(lulog *log, luary_void **ptrs, size_t n);
/// Free an array of void* values.
int luary_void_free(luary_void **ptrs, int prev_status);
/// Allocate space in an array of void* values.
int luary_void_res(lulog *log, luary_void *ptrs, size_t n);
/// Append a value to an array of void* values.
int luary_void_push(lulog *log, luary_void *ptrs, void *ptr);
/// Dump an array of void* values to the log.
int luary_void_dump(lulog *log, luary_void *ptrs, const char *name, size_t nlines);

/// }@
#endif
