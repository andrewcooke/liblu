
#ifndef LU_GL_MATRICES_H
#define LU_GL_MATRICES_H

#include "log.h"
#include "gl_types.h"


// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

void luglm_copy(luglm *a, luglm *b);
void luglm_set(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33,
        luglm *m);
void luglm_set3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22,
        luglm *m);
void luglm_id(luglm *m);
void luglm_zero(luglm *m);
void luglm_rotx(float theta, luglm *m);
void luglm_roty(float theta, luglm *m);
void luglm_rotz(float theta, luglm *m);
void luglm_offset(float x, float y, float z, luglm *m);
void luglm_scale(float k, luglm *m);
int luglm_eq(luglm *a, luglm *b);
int luglm_eq3(luglm *a, luglm *b);
int luglm_approx(luglm *a, luglm *b, float delta);
int luglm_approx3(luglm *a, luglm *b, float delta);
void luglm_mult(luglm *a, luglm *b, luglm *c);
void luglm_mult_inplace(luglm *a, luglm *c);
void luglm_trans(luglm *m, luglm *t);
// this one can fail
int lumat_inv(lulog *log, luglm *m, luglm *i);
int lumat_print(lulog *log, lulog_level level, luglm *m);

#endif
