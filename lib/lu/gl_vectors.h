
#ifndef LU_VECTORS_H
#define LU_VECTORS_H

#include "log.h"
#include "gl_types.h"


// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

//void luglv3_copy(luglv3 *a, luglv3 *b);

void luglv_copy(luglv *a, luglv *b);
void luglv_zero(luglv *v);
int luglv_eq(luglv *a, luglv *b);
int luglv_eq3(luglv *a, luglv *b);
int luglv_approx(luglv *a, luglv *b, float delta);
int luglv_approx3(luglv *a, luglv *b, float delta);
void luglv_add(luglv *a, luglv *b, luglv *c);
void luglv_sub(luglv *a, luglv *b, luglv *c);
void luglv_scale(float k, luglv *v, luglv *c);
void luglv_scale_inplace(float k, luglv *v);
void luglv_cross(luglv *a, luglv *b, luglv *c);
float luglv_dot(luglv *a, luglv *b);
float luglv_len(luglv *a);
void luglv_norm(luglv *a, luglv *c);
void luglv_norm_inplace(luglv *v);
void luglv_mult(luglm *m, luglv *v, luglv *c);
void luglv_rot(luglq *q, luglv *v, luglv *c);
char *luglv_str(luglv *a, int n, char *buffer);
int luvec_prnf4(lulog *log, lulog_level level, luglv *v);

#endif
