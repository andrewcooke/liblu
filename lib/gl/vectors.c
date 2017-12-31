
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


//void luglv3_copy(luglv3 *a, luglv3 *b) {
//    memcpy(b, a, sizeof(*a));
//}


void luglv_copy(luglv *a, luglv *b) {
    memcpy(b, a, sizeof(*a));
}

void luglv_zero(luglv *v) {
    memset(v, 0, sizeof(*v));
}

static inline int vec_eqf_n(luglv *a, luglv *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if ((*a)[i] != (*b)[i]) return 0;
    }
    return 1;
}

int luglv_eq(luglv *a, luglv *b) {
    return vec_eqf_n(a, b, 4);
}

int luglv_eq3(luglv *a, luglv *b) {
    return vec_eqf_n(a, b, 3);
}

static inline int vec_apxf_n(luglv *a, luglv *b, size_t n, float delta) {
    for (size_t i = 0; i < n; ++i) {
        float scaled = delta * max(1, max(fabs((*a)[i]), fabs((*b)[i])));
        if (fabs((*a)[i] - (*b)[i]) > delta) return 0;
    }
    return 1;
}

int luglv_approx(luglv *a, luglv *b, float delta) {
    return vec_apxf_n(a, b, delta, 4);
}

int luglv_approx3(luglv *a, luglv *b, float delta) {
    return vec_apxf_n(a, b, delta, 3);
}

void luglv_add(luglv *a, luglv *b, luglv *c) {
    (*c)[0] = (*a)[0] + (*b)[0];
    (*c)[1] = (*a)[1] + (*b)[1];
    (*c)[2] = (*a)[2] + (*b)[2];
    (*c)[3] = (*a)[3];
}

void luglv_sub(luglv *a, luglv *b, luglv *c) {
    (*c)[0] = (*a)[0] - (*b)[0];
    (*c)[1] = (*a)[1] - (*b)[1];
    (*c)[2] = (*a)[2] - (*b)[2];
    (*c)[3] = (*a)[3];
}

void luglv_scale(float k, luglv *v, luglv *c) {
    (*c)[0] = (*v)[0] * k;
    (*c)[1] = (*v)[1] * k;
    (*c)[2] = (*v)[2] * k;
    (*c)[3] = (*v)[3];
}

void luglv_scale_inplace(float k, luglv *v) {
    (*v)[0] = (*v)[0] * k;
    (*v)[1] = (*v)[1] * k;
    (*v)[2] = (*v)[2] * k;
}

void luglv_cross(luglv *a, luglv *b, luglv *c) {
    (*c)[0] = (*a)[1] * (*b)[2] - (*a)[2] * (*b)[1];
    (*c)[1] = (*a)[2] * (*b)[0] - (*a)[0] * (*b)[2];
    (*c)[2] = (*a)[0] * (*b)[1] - (*a)[1] * (*b)[0];
    (*c)[3] = (*a)[3];
}

float luglv_dot(luglv *a, luglv *b) {
    return (*a)[0] * (*b)[0] + (*a)[1] * (*b)[1] + (*a)[2] * (*b)[2];
}

float luglv_len(luglv *a) {
    return sqrtf((*a)[0] * (*a)[0] + (*a)[1] * (*a)[1] + (*a)[2] * (*a)[2]);
}

void luglv_norm(luglv *a, luglv *c) {
    float l = luglv_len(a);
    (*c)[0] = (*a)[0] / l;
    (*c)[1] = (*a)[1] / l;
    (*c)[2] = (*a)[2] / l;
    (*c)[3] = (*a)[3];
}

void luglv_norm_inplace(luglv *v) {
    float l = luglv_len(v);
    (*v)[0] = (*v)[0] / l;
    (*v)[1] = (*v)[1] / l;
    (*v)[2] = (*v)[2] / l;
}

void luglv_mult(luglm *m, luglv *v, luglv *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[i] += (*m)[luglm_idx(i,j)] * (*v)[j];
        }
    }
}

void luglv_rot(luglq *q, luglv *v, luglv *c) {
    luglq i = {}, w = {}, x = {};
    luglq_inv(q, &i);
    luglv_copy(v, &w); w[3] = 0;
    luglq_mult(q, &w, &x);
    luglq_mult(&x, &i, c);
    (*c)[3] = (*v)[3];
}

char *luglv_str(luglv *a, int n, char *buffer) {
    snprintf(buffer, n, "[%12.6g,%12.6g,%12.6g,%12.6g]", (*a)[0], (*a)[1], (*a)[2], (*a)[3]);
    return buffer;
}

int luvec_log(lulog *log, lulog_level level, luglv *v) {
    return lulog_printf(log, level, "[%12.6g,%12.6g,%12.6g,%12.6g]", (*v)[0], (*v)[1], (*v)[2], (*v)[3]);
}
