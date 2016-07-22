
#include <math.h>
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/dynamic_memory.h"
#include "lu/vectors.h"
#include "lu/minmax.h"


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


void lumat_cpyf4(lumat_f4 *a, lumat_f4 *b) {
    memcpy(b, a, sizeof(*a));
}

void lumat_zrof4(lumat_f4 *m) {
    memset(m, 0, sizeof(*m));
}

static inline int mat_eqf4_n(lumat_f4 *a, lumat_f4 *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if ((*a)[lumat_idx4(i, j)] != (*b)[lumat_idx4(i, j)]) return 0;
        }
    }
    return 1;
}

int lumat_eqf4(lumat_f4 *a, lumat_f4 *b) {
    return mat_eqf4_n(a, b, 4);
}

int lumat_eqf4_3(lumat_f4 *a, lumat_f4 *b) {
    return mat_eqf4_n(a, b, 3);
}

static inline int mat_apxf4_n(lumat_f4 *a, lumat_f4 *b, size_t n, float delta) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            float scaled = delta * max(1, max(fabs((*a)[lumat_idx4(i, j)]), fabs((*b)[lumat_idx4(i, j)])));
            if (fabs((*a)[lumat_idx4(i, j)] - (*b)[lumat_idx4(i, j)]) > delta) return 0;
        }
    }
    return 1;
}

int lumat_apxf4(lumat_f4 *a, lumat_f4 *b, float delta) {
    return mat_apxf4_n(a, b, 4, delta);
}

int lumat_apxf4_3(lumat_f4 *a, lumat_f4 *b, float delta) {
    return mat_apxf4_n(a, b, 3, delta);
}

void lumat_idnf4(lumat_f4 *m) {
    lumat_zrof4(m);
    for (size_t i = 0; i < 4; ++i) (*m)[lumat_idx4(i, i)] = 1;
}

void lumat_setf4(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33,
        lumat_f4 *m) {
    (*m)[lumat_idx4(0,0)] = a00; (*m)[lumat_idx4(1,0)] = a10;
    (*m)[lumat_idx4(0,1)] = a01; (*m)[lumat_idx4(1,1)] = a11;
    (*m)[lumat_idx4(0,2)] = a02; (*m)[lumat_idx4(1,2)] = a12;
    (*m)[lumat_idx4(0,3)] = a03; (*m)[lumat_idx4(1,3)] = a13;
    (*m)[lumat_idx4(2,0)] = a20; (*m)[lumat_idx4(3,0)] = a30;
    (*m)[lumat_idx4(2,1)] = a21; (*m)[lumat_idx4(3,1)] = a31;
    (*m)[lumat_idx4(2,2)] = a22; (*m)[lumat_idx4(3,2)] = a32;
    (*m)[lumat_idx4(2,3)] = a23; (*m)[lumat_idx4(3,3)] = a33;
}

void lumat_setf4_3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22,
        lumat_f4 *m) {
    lumat_setf4(
            a00, a01, a02, 0,
            a10, a11, a12, 0,
            a20, a21, a22, 0,
              0,   0,   0, 1, m);
}

void lumat_mulf4(lumat_f4 *a, lumat_f4 *b, lumat_f4 *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[lumat_idx4(i,j)] = 0;
            for (size_t k = 0; k < 4; ++k) {
                // https://en.wikipedia.org/wiki/Matrix_multiplication#General_definition_of_the_matrix_product
                (*c)[lumat_idx4(i,j)] += (*a)[lumat_idx4(i,k)] * (*b)[lumat_idx4(k,j)];
            }
        }
    }
}

void lumat_mulf4_in(lumat_f4 *a, lumat_f4 *c) {
    lumat_f4 b;
    lumat_cpyf4(c, &b);
    lumat_mulf4(a, &b, c);
}

void lumat_trnf4(lumat_f4 *m, lumat_f4 *t) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*t)[lumat_idx4(i,j)] = (*m)[lumat_idx4(j,i)];
        }
    }
}

int lumat_invf4(lulog *log, lumat_f4 *m, lumat_f4 *i) {

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

void lumat_rotf4_x(float theta, lumat_f4 *m) {
    lumat_setf4_3(1,           0,            0,
                  0, cosf(theta), -sinf(theta),
                  0, sinf(theta),  cosf(theta), m);
}

void lumat_rotf4_y(float theta, lumat_f4 *m) {
    lumat_setf4_3(cosf(theta), 0, sinf(theta),
                            0, 1,           0,
                 -sinf(theta), 0, cosf(theta), m);
}

void lumat_rotf4_z(float theta, lumat_f4 *m) {
    lumat_setf4_3(cosf(theta), -sinf(theta), 0,
                  sinf(theta),  cosf(theta), 0,
                            0,            0, 1, m);
}

void lumat_offf4_3(float x, float y, float z, lumat_f4 *m) {
    lumat_setf4(1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1, m);
}

void lumat_sclf4_3(float k, lumat_f4 *m) {
    lumat_setf4(k, 0, 0, 0,
                0, k, 0, 0,
                0, 0, k, 0,
                0, 0, 0, 1, m);
}


void luvec_cpyf3(luvec_f3 *a, luvec_f3 *b) {
    memcpy(b, a, sizeof(*a));
}


void luvec_cpyf4(luvec_f4 *a, luvec_f4 *b) {
    memcpy(b, a, sizeof(*a));
}

void luvec_zrof4(luvec_f4 *v) {
    memset(v, 0, sizeof(*v));
}

static inline int vec_eqf4_n(luvec_f4 *a, luvec_f4 *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if ((*a)[i] != (*b)[i]) return 0;
    }
    return 1;
}

int luvec_eqf4(luvec_f4 *a, luvec_f4 *b) {
    return vec_eqf4_n(a, b, 4);
}

int luvec_eqf4_3(luvec_f4 *a, luvec_f4 *b) {
    return vec_eqf4_n(a, b, 3);
}

static inline int vec_apxf4_n(luvec_f4 *a, luvec_f4 *b, size_t n, float delta) {
    for (size_t i = 0; i < n; ++i) {
        float scaled = delta * max(1, max(fabs((*a)[i]), fabs((*b)[i])));
        if (fabs((*a)[i] - (*b)[i]) > delta) return 0;
    }
    return 1;
}

int luvec_apxf4(luvec_f4 *a, luvec_f4 *b, float delta) {
    return vec_apxf4_n(a, b, delta, 4);
}

int luvec_apxf4_3(luvec_f4 *a, luvec_f4 *b, float delta) {
    return vec_apxf4_n(a, b, delta, 3);
}

void luvec_addf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c) {
    (*c)[0] = (*a)[0] + (*b)[0];
    (*c)[1] = (*a)[1] + (*b)[1];
    (*c)[2] = (*a)[2] + (*b)[2];
    (*c)[3] = (*a)[3];
}

void luvec_subf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c) {
    (*c)[0] = (*a)[0] - (*b)[0];
    (*c)[1] = (*a)[1] - (*b)[1];
    (*c)[2] = (*a)[2] - (*b)[2];
    (*c)[3] = (*a)[3];
}

void luvec_sclf4_3(float k, luvec_f4 *v, luvec_f4 *c) {
    (*c)[0] = (*v)[0] * k;
    (*c)[1] = (*v)[1] * k;
    (*c)[2] = (*v)[2] * k;
    (*c)[3] = (*v)[3];
}

void luvec_sclf4_3in(float k, luvec_f4 *v) {
    (*v)[0] = (*v)[0] * k;
    (*v)[1] = (*v)[1] * k;
    (*v)[2] = (*v)[2] * k;
}

void luvec_crsf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c) {
    (*c)[0] = (*a)[1] * (*b)[2] - (*a)[2] * (*b)[1];
    (*c)[1] = (*a)[2] * (*b)[0] - (*a)[0] * (*b)[2];
    (*c)[2] = (*a)[0] * (*b)[1] - (*a)[1] * (*b)[0];
    (*c)[3] = (*a)[3];
}

float luvec_dotf4_3(luvec_f4 *a, luvec_f4 *b) {
    return (*a)[0] * (*b)[0] + (*a)[1] * (*b)[1] + (*a)[2] * (*b)[2];
}

float luvec_lenf4_3(luvec_f4 *a) {
    return sqrtf((*a)[0] * (*a)[0] + (*a)[1] * (*a)[1] + (*a)[2] * (*a)[2]);
}

void luvec_nrmf4_3(luvec_f4 *a, luvec_f4 *c) {
    float l = luvec_lenf4_3(a);
    (*c)[0] = (*a)[0] / l;
    (*c)[1] = (*a)[1] / l;
    (*c)[2] = (*a)[2] / l;
    (*c)[3] = (*a)[3];
}

void luvec_nrmf4_3in(luvec_f4 *v) {
    float l = luvec_lenf4_3(v);
    (*v)[0] = (*v)[0] / l;
    (*v)[1] = (*v)[1] / l;
    (*v)[2] = (*v)[2] / l;
}

void luvec_mulf4(lumat_f4 *m, luvec_f4 *v, luvec_f4 *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[i] += (*m)[lumat_idx4(i,j)] * (*v)[j];
        }
    }
}

void luvec_rotf4(luqua_f4 *q, luvec_f4 *v, luvec_f4 *c) {
    luqua_f4 i = {}, w = {}, x = {};
    luqua_invf4(q, &i);
    luvec_cpyf4(v, &w); w[3] = 0;
    luqua_mulf4(q, &w, &x);
    luqua_mulf4(&x, &i, c);
    (*c)[3] = (*v)[3];
}

char *luvec_strf4(luvec_f4 *a, int n, char *buffer) {
    snprintf(buffer, n, "[%12.6g,%12.6g,%12.6g,%12.6g]", (*a)[0], (*a)[1], (*a)[2], (*a)[3]);
    return buffer;
}


void luqua_cpyf4(luqua_f4 *a, luqua_f4 *b) {return luvec_cpyf4(a, b);}

void luqua_zrof4(luqua_f4 *q) {return luvec_zrof4(q);}

void luqua_idnf4(luqua_f4 *q) {
    luqua_zrof4(q);
    (*q)[3] = 1;
}

float luqua_lenf4(luqua_f4 *q) {
    return sqrtf((*q)[0]*(*q)[0] + (*q)[1]*(*q)[1] + (*q)[2]*(*q)[2] + (*q)[3]*(*q)[3]);
}

void luqua_nrmf4_in(luvec_f4 *q) {
    float l = luqua_lenf4(q);
    (*q)[0] = (*q)[0] / l;
    (*q)[1] = (*q)[1] / l;
    (*q)[2] = (*q)[2] / l;
    (*q)[3] = (*q)[3] / l;
}

void luqua_mulf4(luqua_f4 *a, luqua_f4 *b, luqua_f4 *c) {
    (*c)[0] = (*a)[3]*(*b)[0] + (*a)[0]*(*b)[3] + (*a)[1]*(*b)[2] - (*a)[2]*(*b)[1];
    (*c)[1] = (*a)[3]*(*b)[1] + (*a)[1]*(*b)[3] + (*a)[2]*(*b)[0] - (*a)[0]*(*b)[2];
    (*c)[2] = (*a)[3]*(*b)[2] + (*a)[2]*(*b)[3] + (*a)[0]*(*b)[1] - (*a)[1]*(*b)[0];
    (*c)[3] = (*a)[3]*(*b)[3] - (*a)[0]*(*b)[0] - (*a)[1]*(*b)[1] - (*a)[2]*(*b)[2];
}

void luqua_rotf4_x(float theta, luqua_f4 *q) {
    (*q)[0] = sinf(theta/2);
    (*q)[1] = (*q)[2] = 0;
    (*q)[3] = cosf(theta/2);
}

void luqua_rotf4_y(float theta, luqua_f4 *q) {
    (*q)[1] = sinf(theta/2);
    (*q)[2] = (*q)[0] = 0;
    (*q)[3] = cosf(theta/2);
}

void luqua_rotf4_z(float theta, luqua_f4 *q) {
    (*q)[2] = sinf(theta/2);
    (*q)[0] = (*q)[1] = 0;
    (*q)[3] = cosf(theta/2);
}

void luqua_invf4(luqua_f4 *q, luqua_f4 *i) {
    float l = luqua_lenf4(q);
    (*i)[0] = -(*q)[0] / l;
    (*i)[1] = -(*q)[1] / l;
    (*i)[2] = -(*q)[2] / l;
    (*i)[3] = (*q)[3] / l;
}

int luvec_prnf4(lulog *log, lulog_level level, luvec_f4 *v) {
    return lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]", (*v)[0], (*v)[1], (*v)[2], (*v)[3]);
}

int lumat_prnf4(lulog *log, lulog_level level, lumat_f4 *m) {
    LU_STATUS
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[lumat_idx4(0,0)], (*m)[lumat_idx4(0,1)], (*m)[lumat_idx4(0,2)], (*m)[lumat_idx4(0,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[lumat_idx4(1,0)], (*m)[lumat_idx4(1,1)], (*m)[lumat_idx4(1,2)], (*m)[lumat_idx4(1,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[lumat_idx4(2,0)], (*m)[lumat_idx4(2,1)], (*m)[lumat_idx4(2,2)], (*m)[lumat_idx4(2,3)]))
    LU_CHECK(lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]",
            (*m)[lumat_idx4(3,0)], (*m)[lumat_idx4(3,1)], (*m)[lumat_idx4(3,2)], (*m)[lumat_idx4(3,3)]))
    LU_NO_CLEANUP
}
