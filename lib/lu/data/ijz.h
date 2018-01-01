
#ifndef LU_DATA_IJZ_H
#define LU_DATA_IJZ_H

#include "../log.h"
#include "../memory.h"

/**
 * @file
 *
 * @brief Support for (i,j,z) tuples and their arrays.
 */
/// @{

/// An (i,j,z) tuple - typically height at a 2D coordinate.
typedef struct ludta_ijz {
    int32_t i;
    int32_t j;
    double z;
} ludta_ijz;

/// An array of (i,j,z) tuples.
typedef struct luary_ijz {
    ludta_ijz *ijz;
    lumem mem;
} luary_ijz;

/// Create an array of (i,j,z) tuples.
int luary_mkijz(lulog *log, luary_ijz **ijz, size_t n);
/// Free an array of (i,j,z) tuples.
int luary_freeijz(luary_ijz **ijz, int prev_status);
/// Allocate space in an array of (i,j,z) tuples.
int luary_reserveijz(lulog *log, luary_ijz *ijz, size_t n);
/// Append a value to an array of (i,j,z) tuples.
int luary_pushijz(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z);
/// Dump an array of (i,j,z) tuples to the log.
int luary_dumpijz(lulog *log, luary_ijz *ijz, const char *name, size_t nlines);

/// @}
#endif
