
#include <lu/internal.h>
#include <math.h>
#include <string.h>

#include "lu/log.h"
#include "lu/memory.h"
#include "lu/minmax.h"
#include "lu/gl/quaternions.h"
#include "lu/gl/vectors.h"


// the code below is formatted assuming that when multiplying
// matrices and vectors, the vectors are columns.  the actual
// memory layout depends only on lumat_idx4.

// the aim is to (1) follow the usual conventions when
// writing matrix and vector maths and (2) have a memory
// layout consistent with opengl.

// (if you're looking for a general matrix / vector library this
// probably isn't it - it is very much intended to support
// opengl, hence the layout and the use of a fourth (w)
// component.


// memory convention here is that objects are allocated on the
// stack by the caller:
//   lumat_f4 m = {}; luvec_f4 v = {};
// and then passed to these routines using '&'.  this avoids
// tedious messing around with heap memory (but has consequences
// for stack use, obviously...)



void luglq_copy(luglq *a, luglq *b) {return luglv_copy(a, b);}

void luglq_zero(luglq *q) {return luglv_zero(q);}

void luglq_id(luglq *q) {
    luglq_zero(q);
    (*q)[3] = 1;
}

float luglq_len(luglq *q) {
    return sqrtf((*q)[0]*(*q)[0] + (*q)[1]*(*q)[1] + (*q)[2]*(*q)[2] + (*q)[3]*(*q)[3]);
}

void luglq_norm_inplace(luglv *q) {
    float l = luglq_len(q);
    (*q)[0] = (*q)[0] / l;
    (*q)[1] = (*q)[1] / l;
    (*q)[2] = (*q)[2] / l;
    (*q)[3] = (*q)[3] / l;
}

void luglq_mult(luglq *a, luglq *b, luglq *c) {
    (*c)[0] = (*a)[3]*(*b)[0] + (*a)[0]*(*b)[3] + (*a)[1]*(*b)[2] - (*a)[2]*(*b)[1];
    (*c)[1] = (*a)[3]*(*b)[1] + (*a)[1]*(*b)[3] + (*a)[2]*(*b)[0] - (*a)[0]*(*b)[2];
    (*c)[2] = (*a)[3]*(*b)[2] + (*a)[2]*(*b)[3] + (*a)[0]*(*b)[1] - (*a)[1]*(*b)[0];
    (*c)[3] = (*a)[3]*(*b)[3] - (*a)[0]*(*b)[0] - (*a)[1]*(*b)[1] - (*a)[2]*(*b)[2];
}

void luglq_rotx(float theta, luglq *q) {
    (*q)[0] = sinf(theta/2);
    (*q)[1] = (*q)[2] = 0;
    (*q)[3] = cosf(theta/2);
}

void luglq_roty(float theta, luglq *q) {
    (*q)[1] = sinf(theta/2);
    (*q)[2] = (*q)[0] = 0;
    (*q)[3] = cosf(theta/2);
}

void luglq_rotz(float theta, luglq *q) {
    (*q)[2] = sinf(theta/2);
    (*q)[0] = (*q)[1] = 0;
    (*q)[3] = cosf(theta/2);
}

void luglq_inv(luglq *q, luglq *i) {
    float l = luglq_len(q);
    (*i)[0] = -(*q)[0] / l;
    (*i)[1] = -(*q)[1] / l;
    (*i)[2] = -(*q)[2] / l;
    (*i)[3] = (*q)[3] / l;
}

