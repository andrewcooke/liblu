
#ifndef LU_DATA_INT32_H
#define LU_DATA_INT32_H

#include "../log.h"
#include "../memory.h"

/**
 * @file
 *
 * @brief Support for arrays of int32_t values.
 */
/// @{

/// An array of int32_t values.
typedef struct luary_int32 {
    int32_t *i;
    lumem mem;
} luary_int32;

/// Create an array of int32_t values.
int luary_mkint32(lulog *log, luary_int32 **i, size_t n);
/// Free an array of int32_t values.
int luary_freeint32(luary_int32 **i, int prev_status);
/// Allocate space in an array of int32_t values.
int luary_reserveint32(lulog *log, luary_int32 *i, size_t n);
/// Append a value to an array of int32_t values.
int luary_pushint32(lulog *log, luary_int32 *i, int32_t j);
/// Dump an array of int32_t values to the log.
int luary_dumpint32(lulog *log, luary_int32 *i, const char *name, size_t nlines);

/// }@
#endif
