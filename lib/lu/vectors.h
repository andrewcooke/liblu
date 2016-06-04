
#ifndef LU_VECTORS_H
#define LU_VECTORS_H

#include "structs.h"  // todo - remove later
#include "log.h"


// https://fgiesen.wordpress.com/2012/02/12/row-major-vs-column-major-row-vectors-vs-column-vectors/

typedef float lumat_f4[16];
typedef float luvec_f4[4];

// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

#define lumat_idx4(i, j)  ({ __typeof__ (i) _i = (i); __typeof__ (j) _j = (j); 4*_j + _i;})
int lumat_copyf4(lulog *log, lumat_f4 *a, lumat_f4 **b);
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
void lumat_mulf4(lumat_f4 *a, lumat_f4 *b, lumat_f4 *c);
void lumat_trnf4(lumat_f4 *m, lumat_f4 *t);
void lumat_rotf4_x(float theta, lumat_f4 *m);
void lumat_rotf4_y(float theta, lumat_f4 *m);
void lumat_rotf4_z(float theta, lumat_f4 *m);
void lumat_offf4(float x, float y, float z, lumat_f4 *m);

int luvec_copyf4(lulog *log, luvec_f4 *a, luvec_f4 **b);
int luvec_eqf4(luvec_f4 *a, luvec_f4 *b);
int luvec_eqf4_3(luvec_f4 *a, luvec_f4 *b);
int luvec_apxf4(luvec_f4 *a, luvec_f4 *b, float delta);
int luvec_apxf4_3(luvec_f4 *a, luvec_f4 *b, float delta);
void luvec_addf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_subf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_crsf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
float luvec_dotf4_3(luvec_f4 *a, luvec_f4 *b);
float luvec_lenf4_3(luvec_f4 *a);
void luvec_nrmf4_3(luvec_f4 *a, luvec_f4 *c);
void luvec_mulf4(lumat_f4 *m, luvec_f4 *v, luvec_f4 *c);
char *luvec_strf4(luvec_f4 *a, int n, char *buffer);

int lueq4(ludat_fxyzw a, ludat_fxyzw b);
ludat_fxyzw luadd3(ludat_fxyzw a, ludat_fxyzw b);
ludat_fxyzw lusub3(ludat_fxyzw a, ludat_fxyzw b);
ludat_fxyzw lucross3(ludat_fxyzw a, ludat_fxyzw b);
float lulen3(ludat_fxyzw a);
ludat_fxyzw lunorm3(ludat_fxyzw a);
ludat_fxyzw lusetw(ludat_fxyzw a, float w);

#endif
