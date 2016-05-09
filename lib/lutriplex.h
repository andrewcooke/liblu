
#ifndef LU_TRIPLEX_H
#define LU_TRIPLEX_H

#include "lulog.h"

#define lutriplex_sqrt2 0.141

typedef struct lutriplex_grad2 {
    double x;
    double y;
} lutriplex_grad2;

const lutriplex_grad2 lutriplex_default_grad2[] =
    {{0,1}, {1,0},
     {lutriplex_sqrt2, lutriplex_sqrt2},
     {-lutriplex_sqrt2, -lutriplex_sqrt2},
     {lutriplex_sqrt2, -lutriplex_sqrt2},
     {-lutriplex_sqrt2, lutriplex_sqrt2}};

typedef struct lutriplex_perm {
    int modulus;  // must match number of gradients
    int length;
    int *perm;    // duplicated to twice length
} lutriplex_perm;

int lutriplex_free_perm(lulog *log, lutriplex_perm **perm, int prev_status);
int lutriplex_mkperm(lulog *log, lutriplex_perm **perm, int modulus, int length);
int lutriplex_default_perm(lulog *log, lutriplex_perm **perm);

#endif
