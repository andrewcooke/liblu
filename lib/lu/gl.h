
#include "gl_types.h"
#include "gl_vectors.h"
#include "gl_matrices.h"
#include "gl_quaternions.h"
#include "gl_colours.h"

/**
 * @file
 *
 * A single header file that imports all the OpenGL headers.
 *
 * The code is formatted assuming that when multiplying matrices and vectors,
 * the vectors are columns.  The actual memory layout depends only on
 * `lumat_idx4`.
 *
 * The aims of the library are to (1) follow the usual conventions when
 * writing matrix and vector maths and (2) have a memory layout consistent with
 * OpenGL (if you're looking for a general matrix / vector library this
 * probably isn't it - it is very much intended to support OpenGL, hence the
 * layout and the use of a fourth (w) component).
 *
 * The memory convention used is that objects are allocated on the stack by
 * the caller:
 *
 *     lumat_f4 m = {}; luvec_f4 v = {};
 *
 * and then passed to these routines using '&'.  This avoids tedious messing
 * around with heap memory (but has consequences for stack use, obviously...).
 */
