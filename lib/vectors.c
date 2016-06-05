
#include <math.h>
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/dynamic_memory.h"
#include "lu/vectors.h"
#include "lu/minmax.h"


// the idea here is that if lumat_idx4 is defined correctly, everything
// else should follow.

void lumat_cpyf4(lumat_f4 *a, lumat_f4 *b) {
    memcpy(b, a, sizeof(*a));
}

void inline lumat_zrof4(lumat_f4 *m) {
    memset(m, 0, sizeof(*m));
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
            a00, a01, a01, 0,
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

void lumat_trnf4(lumat_f4 *m, lumat_f4 *t) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*t)[lumat_idx4(i,j)] = (*m)[lumat_idx4(j,i)];
        }
    }
}

void lumat_rotf4_x(float theta, lumat_f4 *m) {
    lumat_setf4_3(1,          0,           0,
                  0, cos(theta), -sin(theta),
                  0, sin(theta),  cos(theta), m);
}

void lumat_rotf4_y(float theta, lumat_f4 *m) {
    lumat_setf4_3(cos(theta), 0, sin(theta),
                           0, 1,          0,
                 -sin(theta), 0, cos(theta), m);
}

void lumat_rotf4_z(float theta, lumat_f4 *m) {
    lumat_setf4_3(cos(theta), -sin(theta), 0,
                  sin(theta),  cos(theta), 0,
                           0,           0, 1, m);
}

void lumat_offf4(float x, float y, float z, lumat_f4 *m) {
    lumat_setf4(1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1, m);
}

void lumat_sclf4(float k, lumat_f4 *m) {
    lumat_setf4(k, 0, 0, 0,
                0, k, 0, 0,
                0, 0, k, 0,
                0, 0, 0, 1, m);
}


void luvec_cpyf4(luvec_f4 *a, luvec_f4 *b) {
    memcpy(b, a, sizeof(*a));
}

void inline luvec_zrof4(luvec_f4 *m) {
    memset(m, 0, sizeof(*m));
}

static inline int eqf4_n(luvec_f4 *a, luvec_f4 *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if ((*a)[i] != (*b)[i]) return 0;
    }
    return 1;
}

int luvec_eqf4(luvec_f4 *a, luvec_f4 *b) {
    return eqf4_n(a, b, 4);
}

int luvec_eqf4_3(luvec_f4 *a, luvec_f4 *b) {
    return eqf4_n(a, b, 3);
}

static inline int apxf4_n(luvec_f4 *a, luvec_f4 *b, size_t n, float delta) {
    for (size_t i = 0; i < n; ++i) {
        float scaled = delta * max(1, max(fabs((*a)[i]), fabs((*b)[i])));
        if (fabs((*a)[i] - (*b)[i]) > delta) return 0;
    }
    return 1;
}

int luvec_apxf4(luvec_f4 *a, luvec_f4 *b, float delta) {
    return apxf4_n(a, b, delta, 4);
}

int luvec_apxf4_3(luvec_f4 *a, luvec_f4 *b, float delta) {
    return apxf4_n(a, b, delta, 3);
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
    return sqrt((*a)[0] * (*a)[0] + (*a)[1] * (*a)[1] + (*a)[2] * (*a)[2]);
}

void luvec_nrmf4_3(luvec_f4 *a, luvec_f4 *c) {
    float l = luvec_lenf4_3(a);
    (*c)[0] = (*a)[0] / l;
    (*c)[1] = (*a)[1] / l;
    (*c)[2] = (*a)[2] / l;
    (*c)[3] = (*a)[3];
}

void luvec_mulf4(lumat_f4 *m, luvec_f4 *v, luvec_f4 *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[i] += (*m)[lumat_idx4(i,j)] * (*v)[j];
        }
    }
}

char *luvec_strf4(luvec_f4 *a, int n, char *buffer) {
    snprintf(buffer, n, "[%g,%g,%g,%g]", (*a)[0], (*a)[1], (*a)[2], (*a)[3]);
    return buffer;
}


