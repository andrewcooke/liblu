
#ifndef LU_DATA_FXYZW_H
#define LU_DATA_FXYZW_H

#include "../log.h"
#include "../memory.h"

/**
 * @file
 *
 * @brief Support for (x,y,z,w) tuples of float values and their arrays.
 */
/// @{

/// An (x,y,z,w) tuple of float values - typically a vector in OpenCL.
typedef struct ludta_fxyzw {
    float x;
    float y;
    float z;
    float w;
} ludta_fxyzw;

/// An array of (x,y,z,w) float tuples / vectors.
typedef struct luary_fxyzw {
    ludta_fxyzw *fxyzw;
    lumem mem;
} luary_fxyzw;

/// Create an array of (x,y,z,w) float tuples.
int luary_mkfxyzw(lulog *log, luary_fxyzw **fxyzw, size_t n);
/// Free an array of (x,y,z,w) float tuples.
int luary_freefxyzw(luary_fxyzw **fxyzw, int prev_status);
/// Allocate space in an array of (x,y,z,w) float tuples.
int luary_reservefxyzw(lulog *log, luary_fxyzw *fxyzw, size_t n);
/// Append a value to an array of (x,y,z,w) float tuples.
int luary_pushfxyzw(lulog *log, luary_fxyzw *fxyzw, float x, float y, float z, float w);
/// Dump an array of (x,y,z,w) float tuples to the log.
int luary_dumpfxyzw(lulog *log, luary_fxyzw *fxyzw, const char *name, size_t nlines);
/// Print an (x,y,z,w) float tuple to a previously allocated char buffer.
char *ludta_fxyzw2str(ludta_fxyzw xyzw, int n, char *buffer);

/// @}
#endif
