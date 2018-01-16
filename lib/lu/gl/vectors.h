
#ifndef LU_GL_VECTORS_H
#define LU_GL_VECTORS_H

#include "../log.h"
#include "types.h"

/**
 * @file
 *
 * @brief Float vector operations to support OpenGL programs.
 *
 * All input and output arrays passed by pointer.
 */
/// @{

/// Copy a vector (from a to b).
void luglv_copy(luglv *a, luglv *b);
/// Set all vector components to zero.
void luglv_zero(luglv *v);
/// Check for vector equality (all 4 components) - returns `True` on equality.
int luglv_eq(luglv *a, luglv *b);
/// Check for vector equality (3 spatial components) - returns `True` on equality.
int luglv_eq3(luglv *a, luglv *b);
/// Check for approximate vector equality (all 4 components) - returns `True` on equality.
int luglv_approx(luglv *a, luglv *b, float delta);
/// Check for approximate vector equality (3 spatial components) - returns `True` on equality.
int luglv_approx3(luglv *a, luglv *b, float delta);
/// Add spatial components of two vectors (c[0:2] = a[0:2] + b[0:2]; c[3] = a[3]).
void luglv_add(luglv *a, luglv *b, luglv *c);
/// Subtract spatial components of two vectors (c[0:2] = a[0:2] - b[0:2]; c[3] = a[3]).
void luglv_sub(luglv *a, luglv *b, luglv *c);
/// Scale spatial components of two vectors (c[0:2] = v[0:2] * 3; c[3] = v[3]).
void luglv_scale(float k, luglv *v, luglv *c);
/// Scale spatial components of a vectors in-place.
void luglv_scale_inplace(float k, luglv *v);
/// Cross product of the spatial components of two vectors (c[0:2] = a[0:2] x b[0:2]; c[3] = a[3]).
void luglv_cross(luglv *a, luglv *b, luglv *c);
/// Dot (scalar) product of the spatial components of two vectors (a[0:2] , b[0:2]).
float luglv_dot(luglv *a, luglv *b);
/// Spatial length of a vector.
float luglv_len(luglv *a);
/// Copy and normalise a vector (by spatial length).
void luglv_norm(luglv *a, luglv *c);
/// Normalise a vector in-place (by spatial length).
void luglv_norm_inplace(luglv *v);
/// Multiply a matrix and vector.
void luglv_mult(luglm *m, luglv *v, luglv *c);
/// Rotate a vector according to a quaternion (pre-multiply by the quaternion and post-multiply by the inverse).
void luglv_rot(luglq *q, luglv *v, luglv *c);
/// Print a vector to the log.
int luvec_log(lulog *log, lulog_level level, luglv *v);

// TODO - ugly
char *luglv_str(luglv *a, int n, char *buffer);

/// @}
#endif
