
#ifndef LU_TRIPLEX_H
#define LU_TRIPLEX_H

#include "lulog.h"
#include "lurand.h"

struct lutriplex_xy;

typedef struct lutriplex_config {
    int n_grad;
    int n_perm;
    struct lutriplex_xy *grad;
    int *perm;
} lutriplex_config;

int lutriplex_free_config(lutriplex_config **config, int prev_status);
int lutriplex_mkconfig(lulog *log, lurand *rand, lutriplex_config **config,
        int n_grad, double phase, int n_perm);
int lutriplex_default_config(lulog *log, lutriplex_config **config);

typedef struct lutriplex_xy {
    double x;
    double y;
} lutriplex_xy;

typedef struct lutriplex_xys {
    lutriplex_xy *xy;
    lumem mem;
} lutriplex_xys;

int lutriplex_mkxyn(lulog *log, lutriplex_xys **xys, int n);
int lutriplex_freexy(lutriplex_xys **xys, int prev_status);
int lutriplex_reservexy(lulog *log, lutriplex_xys *xys, int n);
int lutriplex_pushxy(lulog *log, lutriplex_xys *xys, double x, double y);

typedef struct lutriplex_xyz {
    double x;
    double y;
    double z;
} lutriplex_xyz;

typedef struct lutriplex_xyzs {
    lutriplex_xyz *xyz;
    lumem mem;
} lutriplex_xyzs;

int lutriplex_mkxyzn(lulog *log, lutriplex_xyzs **xyzs, int n);
int lutriplex_freexyz(lutriplex_xyzs **xys, int prev_status);
int lutriplex_reservexyz(lulog *log, lutriplex_xyzs *xyzs, int n);
int lutriplex_pushxyz(lulog *log, lutriplex_xyzs *xyzs, double x, double y, double z);

struct lutriplex_tile;

typedef int lutriplex_enumerate_xy(struct lutriplex_tile *tile, lulog *log, lutriplex_xys **xys);
typedef int lutriplex_enumerate_xyz(struct lutriplex_tile *tile, lulog *log,
        lutriplex_xyzs **xysz, lutriplex_config *config);
typedef int lutriplex_freetile(struct lutriplex_tile **tile, int prev_status);

typedef struct lutriplex_tile {
    lutriplex_enumerate_xy *enumerate_xy;
    lutriplex_enumerate_xyz *enumerate_xyz;
    lutriplex_freetile *free;
    void *state;
} lutriplex_tile;

int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile, int side, int subsamples);

#endif
