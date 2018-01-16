
#ifndef LU_GL_MATRICES_H
#define LU_GL_MATRICES_H

#include "../log.h"
#include "types.h"

/**
 * @file
 *
 * @brief Float matrix operations to support OpenGL programs.
 *
 * All input and output arrays passed by pointer.
 */
/// @{

/// Copy a matrix (from a to b).
void luglm_copy(luglm *a, luglm *b);
/// Set the values in a matrix, formatted "as written".
void luglm_set(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33,
        luglm *m);
/// Set the "spatial" values in a matrix (others are set as the identity).
void luglm_set3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22,
        luglm *m);
/// Set the matrix values to the identity matrix.
void luglm_id(luglm *m);
/// Set the matrix values to zero (including non-spatial).
void luglm_zero(luglm *m);
/// Generate a matrix for rotating a vector `theta` radians about the x axis.
void luglm_rotx(float theta, luglm *m);
/// Generate a matrix for rotating a vector `theta` radians about the y axis.
void luglm_roty(float theta, luglm *m);
/// Generate a matrix for rotating a vector `theta` radians about the z axis.
void luglm_rotz(float theta, luglm *m);
/// Generate a matrix for off-setting (translating) a vector by `(x,y,z)`.
void luglm_offset(float x, float y, float z, luglm *m);
/// Generate a matrix for scaling the spatial dimensions of a vector by `k`.
void luglm_scale(float k, luglm *m);
/// Check for matrix equality (all 16 components) - returns `True` on equality.
int luglm_eq(luglm *a, luglm *b);
/// Check for matrix equality (spatial components) - returns `True` on equality.
int luglm_eq3(luglm *a, luglm *b);
/// Check for approximate matrix equality (all 16 components) - returns `True` on equality.
int luglm_approx(luglm *a, luglm *b, float delta);
/// Check for approximate matrix equality (spatial components) - returns `True` on equality.
int luglm_approx3(luglm *a, luglm *b, float delta);
/// Multiply two matrices to generate a third (c = ab).
void luglm_mult(luglm *a, luglm *b, luglm *c);
/// Multiply two matrices to generate a third in-place (c = ac).
void luglm_mult_inplace(luglm *a, luglm *c);
/// Transpose a matrix.
void luglm_trans(luglm *m, luglm *t);
// Invert a matrix (note - this may fail and returns a status code).
int lumat_inv(lulog *log, luglm *m, luglm *i);
/// Print a matrix to the log.
int lumat_log(lulog *log, lulog_level level, luglm *m);

#endif
