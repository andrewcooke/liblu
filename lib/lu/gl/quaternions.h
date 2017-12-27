
#ifndef LU_GL_QUATERNIONS_H
#define LU_GL_QUATERNIONS_H

#include "../log.h"
#include "types.h"

/**
 * @file
 *
 * Quaternion operations to support OpenGL programs.
 *
 * All input and output arrays passed by pointer.
 */
/// @{

/// Copy a quaternion (from a to b).
void luglq_copy(luglq *a, luglq *b);
/// Set all quaternion components to zero.
void luglq_zero(luglq *q);
/// Set the unit quaternion.
void luglq_id(luglq *q);
/// The length of the quaternion.
float luglq_len(luglq *q);
/// Normalise the quaternion in-place.
void luglq_norm_inplace(luglv *q);
/// Multiple two quaternions (c = ab).
void luglq_mult(luglq *a, luglq *b, luglq *c);
/// Generate a quaternion that represents a rotation of `theta` radians about the x axis.
void luglq_rotx(float theta, luglq *q);
/// Generate a quaternion that represents a rotation of `theta` radians about the y axis.
void luglq_roty(float theta, luglq *q);
/// Generate a quaternion that represents a rotation of `theta` radians about the z axis.
void luglq_rotz(float theta, luglq *q);
/// Calculate the inverse of a quaternion.
void luglq_inv(luglq *q, luglq *i);

#endif
