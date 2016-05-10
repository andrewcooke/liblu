
#ifndef LU_TRIPLEX_H
#define LU_TRIPLEX_H

#include "lulog.h"
#include "lurand.h"

typedef struct lutriplex_grad2 {
    double x;
    double y;
} lutriplex_grad2;

typedef struct lutriplex_config2 {
    int n_grad;
    int n_perm;
    lutriplex_grad2 *grad;
    int *perm;
} lutriplex_config2;

int lutriplex_free_config2(lutriplex_config2 **config, int prev_status);
int lutriplex_mkconfig2(lulog *log, lurand *rand, lutriplex_config2 **config,
        int n_grad, double phase, int n_perm);
int lutriplex_default_config2(lulog *log, lutriplex_config2 **config);

#endif
