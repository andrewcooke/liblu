
#ifndef LU_GL_QUATERNIONS_H
#define LU_GL_QUATERNIONS_H

#include "log.h"
#include "gl_types.h"


// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

void luglq_copy(luglq *a, luglq *b);
void luglq_zero(luglq *q);
void luglq_id(luglq *q);
float luglq_len(luglq *q);
void luglq_norm_inplace(luglv *q);
void luglq_mult(luglq *a, luglq *b, luglq *c);
void luglq_rotx(float theta, luglq *q);
void luglq_roty(float theta, luglq *q);
void luglq_rotz(float theta, luglq *q);
void luglq_inv(luglq *q, luglq *i);

#endif
