
#ifndef LU_VECTORS_H
#define LU_VECTORS_H

#include "log.h"


// https://fgiesen.wordpress.com/2012/02/12/row-major-vs-column-major-row-vectors-vs-column-vectors/
// column order (fortran), first fastest, so j is scaled
#define lumat_idx4(i, j)  ({ __typeof__ (i) _i = (i); __typeof__ (j) _j = (j); 4 * _j + _i;})

typedef float lumat_f4[16];
typedef float luvec_f3[3];
typedef float luvec_f4[4];
typedef float luqua_f4[4];

// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

void lumat_cpyf4(lumat_f4 *a, lumat_f4 *b);
void lumat_setf4(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33,
        lumat_f4 *m);
void lumat_setf4_3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22,
        lumat_f4 *m);
void lumat_idnf4(lumat_f4 *m);
void lumat_zrof4(lumat_f4 *m);
void lumat_rotf4_x(float theta, lumat_f4 *m);
void lumat_rotf4_y(float theta, lumat_f4 *m);
void lumat_rotf4_z(float theta, lumat_f4 *m);
void lumat_offf4_3(float x, float y, float z, lumat_f4 *m);
void lumat_sclf4_3(float k, lumat_f4 *m);
int lumat_eqf4(lumat_f4 *a, lumat_f4 *b);
int lumat_eqf4_3(lumat_f4 *a, lumat_f4 *b);
int lumat_apxf4(lumat_f4 *a, lumat_f4 *b, float delta);
int lumat_apxf4_3(lumat_f4 *a, lumat_f4 *b, float delta);
void lumat_mulf4(lumat_f4 *a, lumat_f4 *b, lumat_f4 *c);
void lumat_mulf4_in(lumat_f4 *a, lumat_f4 *c);
void lumat_trnf4(lumat_f4 *m, lumat_f4 *t);
// this one can fail
int lumat_invf4(lulog *log, lumat_f4 *m, lumat_f4 *i);
int lumat_prnf4(lulog *log, lulog_level level, lumat_f4 *m);

void luvec_cpyf3(luvec_f3 *a, luvec_f3 *b);

void luvec_cpyf4(luvec_f4 *a, luvec_f4 *b);
void luvec_zrof4(luvec_f4 *v);
int luvec_eqf4(luvec_f4 *a, luvec_f4 *b);
int luvec_eqf4_3(luvec_f4 *a, luvec_f4 *b);
int luvec_apxf4(luvec_f4 *a, luvec_f4 *b, float delta);
int luvec_apxf4_3(luvec_f4 *a, luvec_f4 *b, float delta);
void luvec_addf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_subf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_sclf4_3(float k, luvec_f4 *v, luvec_f4 *c);
void luvec_sclf4_3in(float k, luvec_f4 *v);
void luvec_crsf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
float luvec_dotf4_3(luvec_f4 *a, luvec_f4 *b);
float luvec_lenf4_3(luvec_f4 *a);
void luvec_nrmf4_3(luvec_f4 *a, luvec_f4 *c);
void luvec_nrmf4_3in(luvec_f4 *v);
void luvec_mulf4(lumat_f4 *m, luvec_f4 *v, luvec_f4 *c);
void luvec_rotf4(luqua_f4 *q, luvec_f4 *v, luvec_f4 *c);
char *luvec_strf4(luvec_f4 *a, int n, char *buffer);
int luvec_prnf4(lulog *log, lulog_level level, luvec_f4 *v);

void luqua_cpyf4(luqua_f4 *a, luqua_f4 *b);
void luqua_zrof4(luqua_f4 *q);
void luqua_idnf4(luqua_f4 *q);
float luqua_lenf4(luqua_f4 *q);
void luqua_nrmf4_in(luvec_f4 *q);
void luqua_mulf4(luqua_f4 *a, luqua_f4 *b, luqua_f4 *c);
void luqua_rotf4_x(float theta, luqua_f4 *q);
void luqua_rotf4_y(float theta, luqua_f4 *q);
void luqua_rotf4_z(float theta, luqua_f4 *q);
void luqua_invf4(luqua_f4 *q, luqua_f4 *i);

#endif
