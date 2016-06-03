
#include <math.h>
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/dynamic_memory.h"
#include "lu/vectors.h"


int lueq4(ludata_fxyzw a, ludata_fxyzw b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

ludata_fxyzw luadd3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){a.x+b.x, a.y+b.y, a.z+b.z, a.w};
}

ludata_fxyzw lusub3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){a.x-b.x, a.y-b.y, a.z-b.z, a.w};
}

ludata_fxyzw lucross3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){(a.y*b.z)-(a.z*b.y), -(a.x*b.z)+(a.z*b.x), (a.x*b.y)-(a.y*b.x), a.w};
}

float lulen3(ludata_fxyzw a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

ludata_fxyzw lunorm3(ludata_fxyzw a) {
    float l = lulen3(a);
    return (ludata_fxyzw){a.x/l, a.y/l, a.z/l, a.w};
}

ludata_fxyzw lusetw(ludata_fxyzw a, float w) {
    return (ludata_fxyzw){a.x, a.y, a.z, w};
}








int lumat_copyf4(lulog *log, lumat_f4 *a, lumat_f4 **b) {
    LU_STATUS
    LU_ALLOC(log, *b, 1)
    memcpy(*b, a, sizeof(*a));
    LU_NO_CLEANUP
}

// the idea here is that if lumat_idxf4 is defined correctly, everything
// else should follow.

void lumat_setf4(lumat_f4 *m,
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33) {
    (*m)[lumat_idxf4(0,0)] = a00; (*m)[lumat_idxf4(1,0)] = a10;
    (*m)[lumat_idxf4(0,1)] = a01; (*m)[lumat_idxf4(1,1)] = a11;
    (*m)[lumat_idxf4(0,2)] = a02; (*m)[lumat_idxf4(1,2)] = a12;
    (*m)[lumat_idxf4(0,3)] = a03; (*m)[lumat_idxf4(1,3)] = a13;
    (*m)[lumat_idxf4(2,0)] = a20; (*m)[lumat_idxf4(3,0)] = a30;
    (*m)[lumat_idxf4(2,1)] = a21; (*m)[lumat_idxf4(3,1)] = a31;
    (*m)[lumat_idxf4(2,2)] = a22; (*m)[lumat_idxf4(3,2)] = a32;
    (*m)[lumat_idxf4(2,3)] = a23; (*m)[lumat_idxf4(3,3)] = a33;
}

void lumat_mulf4(lumat_f4 *a, lumat_f4 *b, lumat_f4 *c) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*c)[lumat_idxf4(i,j)] = 0;
            for (size_t k = 0; k < 4; ++k) {
                // https://en.wikipedia.org/wiki/Matrix_multiplication#General_definition_of_the_matrix_product
                (*c)[lumat_idxf4(i,j)] += (*a)[lumat_idxf4(i,k)] * (*b)[lumat_idxf4(k,j)];
            }
        }
    }
}

void lumat_trnf4(lumat_f4 *m, lumat_f4 *t) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            (*t)[lumat_idxf4(i,j)] = (*m)[lumat_idxf4(j,i)];
        }
    }
}


int luvec_copyf4(lulog *log, luvec_f4 *a, luvec_f4 **b) {
    LU_STATUS
    LU_ALLOC(log, *b, 1)
    memcpy(*b, a, sizeof(*a));
    LU_NO_CLEANUP
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
            (*c)[i] += (*m)[lumat_idxf4(i,j)] * (*v)[j];
        }
    }
}

char *luvec_strf4(luvec_f4 *a, int n, char *buffer) {
    snprintf(buffer, n, "[%g,%g,%g,%g]", (*a)[0], (*a)[1], (*a)[2], (*a)[3]);
    return buffer;
}


