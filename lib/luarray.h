
#ifndef LU_ARRAY_H
#define LU_ARRAY_H

#include "lumem.h"
#include "ludata.h"

typedef struct luarray_xy {
    ludata_xy *xy;
    lumem mem;
} luarray_xy;

int luarray_mkxyn(lulog *log, luarray_xy **xy, int n);
int luarray_freexy(luarray_xy **xy, int prev_status);
int luarray_reservexy(lulog *log, luarray_xy *xy, int n);
int luarray_pushxy(lulog *log, luarray_xy *xy, double x, double y);

typedef struct luarray_xyz {
    ludata_xyz *xyz;
    lumem mem;
} luarray_xyz;

int luarray_mkxyzn(lulog *log, luarray_xyz **xyz, int n);
int luarray_freexyz(luarray_xyz **xy, int prev_status);
int luarray_reservexyz(lulog *log, luarray_xyz *xyz, int n);
int luarray_pushxyz(lulog *log, luarray_xyz *xyz, double x, double y, double z);

#endif
