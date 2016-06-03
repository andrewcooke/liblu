
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

int lumat_copyf4(lulog *log, lumat_f4 *a, lumat_f4 **b);
void lumat_setf4(lumat_f4 *m,
        float a11, float a12, float a13, float a14,
        float a21, float a22, float a23, float a24,
        float a31, float a32, float a33, float a34,
        float a41, float a42, float a43, float a44);
void lumat_mulf4(lumat_f4 *a, lumat_f4 *b, lumat_f4 *c);
void lumat_trnf4(lumat_f4 *m, lumat_f4 *t);
#define lumat_idxf4(i, j)  ({ __typeof__ (i) _i = (i); __typeof__ (j) _j = (j); 4*_j + _i;})
//char *lumat_strf4(lumat_f4 *a, int n, char *buffer);

int luvec_copyf4(lulog *log, luvec_f4 *a, luvec_f4 **b);
int luvec_eqf4(luvec_f4 *a, luvec_f4 *b);
int luvec_eqf4_3(luvec_f4 *a, luvec_f4 *b);
void luvec_addf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_subf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
void luvec_crsf4_3(luvec_f4 *a, luvec_f4 *b, luvec_f4 *c);
float luvec_dotf4_3(luvec_f4 *a, luvec_f4 *b);
float luvec_lenf4_3(luvec_f4 *a);
void luvec_nrmf4_3(luvec_f4 *a, luvec_f4 *c);
void luvec_mulf4(lumat_f4 *m, luvec_f4 *v, luvec_f4 *c);
char *luvec_strf4(luvec_f4 *a, int n, char *buffer);

int lueq4(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw luadd3(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw lusub3(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw lucross3(ludata_fxyzw a, ludata_fxyzw b);
float lulen3(ludata_fxyzw a);
ludata_fxyzw lunorm3(ludata_fxyzw a);
ludata_fxyzw lusetw(ludata_fxyzw a, float w);

#endif
