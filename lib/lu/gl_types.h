
#ifndef LU_GL_TYPES_H
#define LU_GL_TYPES_H

#include "log.h"

// https://fgiesen.wordpress.com/2012/02/12/row-major-vs-column-major-row-vectors-vs-column-vectors/
#define luglm_idx(i, j)  ({ __typeof__ (i) _i = (i); __typeof__ (j) _j = (j); 4 * _j + _i;})

typedef float luglc[3];
typedef float luglm[16];
typedef float luglv[4];
typedef float luglq[4];

#endif
