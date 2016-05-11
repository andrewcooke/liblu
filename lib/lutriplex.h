
#ifndef LU_TRIPLEX_H
#define LU_TRIPLEX_H

#include "lulog.h"
#include "lurand.h"
#include "luarray.h"

struct lutriplex_xy;

typedef struct lutriplex_config {
    int n_grad;
    int n_perm;
    ludata_xy *grad;
    int *perm;
} lutriplex_config;

int lutriplex_free_config(lutriplex_config **config, int prev_status);
int lutriplex_mkconfig(lulog *log, lurand *rand, lutriplex_config **config,
        int n_grad, double phase, int n_perm);
int lutriplex_default_config(lulog *log, lutriplex_config **config);

struct lutriplex_tile;

typedef int lutriplex_enumerate_xy(struct lutriplex_tile *tile, lulog *log, luarray_xy **xy);
typedef int lutriplex_enumerate_xyz(struct lutriplex_tile *tile, lulog *log,
        luarray_xyz **xys, lutriplex_config *config);
typedef int lutriplex_freetile(struct lutriplex_tile **tile, int prev_status);

typedef struct lutriplex_tile {
    ludata_xy centre;
    double side;
    int subsamples;
    lutriplex_enumerate_xy *enumerate_xy;
    lutriplex_enumerate_xyz *enumerate_xyz;
    lutriplex_freetile *free;
    void *state;
} lutriplex_tile;

int lutriplex_mktriangle(lulog *log, lutriplex_tile **tile,
        double x, double y, double side, int subsamples);
int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile,
        double x, double y, double side, int subsamples);

#endif
