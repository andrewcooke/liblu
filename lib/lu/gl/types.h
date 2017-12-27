
#ifndef LU_GL_TYPES_H
#define LU_GL_TYPES_H

#include "../log.h"


/**
 * @file
 *
 * Types used in the support libraries for OpenGL programs.  These are
 * collected here so that the support libraries can easily refer to each
 * other's types as necessary.  All are currently aliases for arrays of
 * `float` values.
 */
/// @{

/// A colour (RGB components).
typedef float luglc[3];
/// A matrix (indexed via luglm_idx).
typedef float luglm[16];
/// A vector.
typedef float luglv[4];
/// A quaternion.
typedef float luglq[4];

/// Indexing for matrices - see https://fgiesen.wordpress.com/2012/02/12/row-major-vs-column-major-row-vectors-vs-column-vectors/
#define luglm_idx(i, j)  ({ __typeof__ (i) _i = (i); __typeof__ (j) _j = (j); 4 * _j + _i;})

#endif
