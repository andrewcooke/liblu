
#include <math.h>
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/dynamic_memory.h"
#include "lu/minmax.h"
#include "lu/gl/matrices.h"


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


void luglm_copy(luglm *a, luglm *b) {
    memcpy(b, a, sizeof(*a));
}

void luglm_zero(luglm *m) {
    memset(m, 0, sizeof(*m));
}

static inline int mat_eqf4_n(luglm *a, luglm *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if ((*a)[luglm_idx(i, j)] != (*b)[luglm_idx(i, j)]) return 0;
        }
    }
    return 1;
}

int luglm_eq(luglm *a, luglm *b) {
    return mat_eqf4_n(a, b, 4);
}

int luglm_eq3(luglm *a, luglm *b) {
    return mat_eqf4_n(a, b, 3);
}

static inline int mat_apxf4_n(luglm *a, luglm *b, size_t n, float delta) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            float scaled = delta * max(1, max(fabs((*a)[luglm_idx(i, j)]), fabs((*b)[luglm_idx(i, j)])));
            if (fabs((*a)[luglm_idx(i, j)] - (*b)[luglm_idx(i, j)]) > delta) return 0;
        }
    }
    return 1;
}

int luglm_approx(luglm *a, luglm *b, float delta) {
    return mat_apxf4_n(a, b, 4, delta);
}

int luglm_approx3(luglm *a, luglm *b, float delta) {
    return mat_apxf4_n(a, b, 3, delta);
}

void luglm_id(luglm *m) {
    luglm_zero(m);
    for (size_t i = 0; i < 4; ++i) (*m)[luglm_idx(i, i)] = 1;
}

void luglm_set(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33,
        luglm *m) {
    (*m)[luglm_idx(0,0)] = a00; (*m)[luglm_idx(1,0)] = a10;
    (*m)[luglm_idx(0,1)] = a01; (*m)[luglm_idx(1,1)] = a11;
    (*m)[luglm_idx(0,2)] = a02; (*m)[luglm_idx(1,2)] = a12;
    (*m)[luglm_idx(0,3)] = a03; (*m)[luglm_idx(1,3)] = a13;
    (*m)[luglm_idx(2,0)] = a20; (*m)[luglm_idx(3,0)] = a30;
    (*m)[luglm_idx(2,1)] = a21; (*m)[luglm_idx(3,1)] = a31;
    (*m)[luglm_idx(2,2)] = a22; (*m)[luglm_idx(3,2)] = a32;
    (*m)[luglm_idx(2,3)] = a23; (*m)[luglm_idx(3,3)] = a33;
}

void luglm_set3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22,
        luglm *m) {
    luglm_set(
            a00, a01, a02, 0,
            a10, a11, a12, 0,
            a20, a21, a22, 0,
              0,   0,   0, 1, m);
}

void luglm_mult(luglm *a, luglm *b, luglm *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[luglm_idx(i,j)] = 0;
            for (size_t k = 0; k < 4; ++k) {
                // https://en.wikipedia.org/wiki/Matrix_multiplication#General_definition_of_the_matrix_product
                (*c)[luglm_idx(i,j)] += (*a)[luglm_idx(i,k)] * (*b)[luglm_idx(k,j)];
            }
        }
    }
}

void luglm_mult_inplace(luglm *a, luglm *c) {
    luglm b;
    luglm_copy(c, &b);
    luglm_mult(a, &b, c);
}

void luglm_trans(luglm *m, luglm *t) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*t)[luglm_idx(i,j)] = (*m)[luglm_idx(j,i)];
        }
    }
}

int lumat_inv(lulog *log, luglm *m, luglm *i) {

    LU_STATUS

    // https://groups.google.com/forum/#!msg/comp.graphics.api.opengl/II36ezVViEI/XwmRKk1aKWkJ
    (*i)[0] = (*m)[5]*((*m)[10]*(*m)[15] - (*m)[11]*(*m)[14]) - (*m)[9]*((*m)[6]*(*m)[15] - (*m)[7]*(*m)[14]) - (*m)[13]*((*m)[7]*(*m)[10] - (*m)[6]*(*m)[11]);
    (*i)[1] = (*m)[1]*((*m)[11]*(*m)[14] - (*m)[10]*(*m)[15]) - (*m)[9]*((*m)[3]*(*m)[14] - (*m)[2]*(*m)[15]) - (*m)[13]*((*m)[2]*(*m)[11] - (*m)[3]*(*m)[10]);
    (*i)[2] = (*m)[1]*((*m)[ 6]*(*m)[15] - (*m)[ 7]*(*m)[14]) - (*m)[5]*((*m)[2]*(*m)[15] - (*m)[3]*(*m)[14]) - (*m)[13]*((*m)[3]*(*m)[ 6] - (*m)[2]*(*m)[ 7]);
    (*i)[3] = (*m)[1]*((*m)[ 7]*(*m)[10] - (*m)[ 6]*(*m)[11]) - (*m)[5]*((*m)[3]*(*m)[10] - (*m)[2]*(*m)[11]) - (*m)[ 9]*((*m)[2]*(*m)[ 7] - (*m)[3]*(*m)[ 6]);

    (*i)[4] = (*m)[4]*((*m)[11]*(*m)[14] - (*m)[10]*(*m)[15]) - (*m)[8]*((*m)[7]*(*m)[14] - (*m)[6]*(*m)[15]) - (*m)[12]*((*m)[6]*(*m)[11] - (*m)[7]*(*m)[10]);
    (*i)[5] = (*m)[0]*((*m)[10]*(*m)[15] - (*m)[11]*(*m)[14]) - (*m)[8]*((*m)[2]*(*m)[15] - (*m)[3]*(*m)[14]) - (*m)[12]*((*m)[3]*(*m)[10] - (*m)[2]*(*m)[11]);
    (*i)[6] = (*m)[0]*((*m)[ 7]*(*m)[14] - (*m)[ 6]*(*m)[15]) - (*m)[4]*((*m)[3]*(*m)[14] - (*m)[2]*(*m)[15]) - (*m)[12]*((*m)[2]*(*m)[ 7] - (*m)[3]*(*m)[ 6]);
    (*i)[7] = (*m)[0]*((*m)[ 6]*(*m)[11] - (*m)[ 7]*(*m)[10]) - (*m)[4]*((*m)[2]*(*m)[11] - (*m)[3]*(*m)[10]) - (*m)[ 8]*((*m)[3]*(*m)[ 6] - (*m)[2]*(*m)[ 7]);

    (*i)[8]  = (*m)[4]*((*m)[ 9]*(*m)[15] - (*m)[11]*(*m)[13]) - (*m)[8]*((*m)[5]*(*m)[15] - (*m)[7]*(*m)[13]) - (*m)[12]*((*m)[7]*(*m)[ 9] - (*m)[5]*(*m)[11]);
    (*i)[9]  = (*m)[0]*((*m)[11]*(*m)[13] - (*m)[ 9]*(*m)[15]) - (*m)[8]*((*m)[3]*(*m)[13] - (*m)[1]*(*m)[15]) - (*m)[12]*((*m)[1]*(*m)[11] - (*m)[3]*(*m)[ 9]);
    (*i)[10] = (*m)[0]*((*m)[ 5]*(*m)[15] - (*m)[ 7]*(*m)[13]) - (*m)[4]*((*m)[1]*(*m)[15] - (*m)[3]*(*m)[13]) - (*m)[12]*((*m)[3]*(*m)[ 5] - (*m)[1]*(*m)[ 7]);
    (*i)[11] = (*m)[0]*((*m)[ 7]*(*m)[ 9] - (*m)[ 5]*(*m)[11]) - (*m)[4]*((*m)[3]*(*m)[ 9] - (*m)[1]*(*m)[11]) - (*m)[ 8]*((*m)[1]*(*m)[ 7] - (*m)[3]*(*m)[ 5]);

    (*i)[12] = (*m)[4]*((*m)[10]*(*m)[13] - (*m)[ 9]*(*m)[14]) - (*m)[8]*((*m)[6]*(*m)[13] - (*m)[5]*(*m)[14]) - (*m)[12]*((*m)[5]*(*m)[10] - (*m)[6]*(*m)[ 9]);
    (*i)[13] = (*m)[0]*((*m)[ 9]*(*m)[14] - (*m)[10]*(*m)[13]) - (*m)[8]*((*m)[1]*(*m)[14] - (*m)[2]*(*m)[13]) - (*m)[12]*((*m)[2]*(*m)[ 9] - (*m)[1]*(*m)[10]);
    (*i)[14] = (*m)[0]*((*m)[ 6]*(*m)[13] - (*m)[ 5]*(*m)[14]) - (*m)[4]*((*m)[2]*(*m)[13] - (*m)[1]*(*m)[14]) - (*m)[12]*((*m)[1]*(*m)[ 6] - (*m)[2]*(*m)[ 5]);
    (*i)[15] = (*m)[0]*((*m)[ 5]*(*m)[10] - (*m)[ 6]*(*m)[ 9]) - (*m)[4]*((*m)[1]*(*m)[10] - (*m)[2]*(*m)[ 9]) - (*m)[ 8]*((*m)[2]*(*m)[ 5] - (*m)[1]*(*m)[ 6]);

    float det = (*m)[0] * (*i)[0] + (*m)[1] * (*i)[4] + (*m)[2] * (*i)[8] + (*m)[3] * (*i)[12];
    LU_ASSERT(det, LU_ERR_MAT, log, "Zero determinant - cannot invert")
    for (size_t j = 0; j < 16; j++) (*i)[j] = (*i)[j] / det;

    LU_NO_CLEANUP
}

void luglm_rotx(float theta, luglm *m) {
    luglm_set3(1,           0,            0,
                  0, cosf(theta), -sinf(theta),
                  0, sinf(theta),  cosf(theta), m);
}

void luglm_roty(float theta, luglm *m) {
    luglm_set3(cosf(theta), 0, sinf(theta),
                            0, 1,           0,
                 -sinf(theta), 0, cosf(theta), m);
}

void luglm_rotz(float theta, luglm *m) {
    luglm_set3(cosf(theta), -sinf(theta), 0,
                  sinf(theta),  cosf(theta), 0,
                            0,            0, 1, m);
}

void luglm_offset(float x, float y, float z, luglm *m) {
    luglm_set(1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1, m);
}

void luglm_scale(float k, luglm *m) {
    luglm_set(k, 0, 0, 0,
                0, k, 0, 0,
                0, 0, k, 0,
                0, 0, 0, 1, m);
}

int lumat_log(lulog *log, lulog_level level, luglm *m) {
    LU_STATUS
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[luglm_idx(0,0)], (*m)[luglm_idx(0,1)], (*m)[luglm_idx(0,2)], (*m)[luglm_idx(0,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[luglm_idx(1,0)], (*m)[luglm_idx(1,1)], (*m)[luglm_idx(1,2)], (*m)[luglm_idx(1,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[luglm_idx(2,0)], (*m)[luglm_idx(2,1)], (*m)[luglm_idx(2,2)], (*m)[luglm_idx(2,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[luglm_idx(3,0)], (*m)[luglm_idx(3,1)], (*m)[luglm_idx(3,2)], (*m)[luglm_idx(3,3)]))
    LU_NO_CLEANUP
}
