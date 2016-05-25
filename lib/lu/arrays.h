
#ifndef LU_ARRAYS_H
#define LU_ARRAYS_H

#include "dynamic_memory.h"
#include "structs.h"


// there's nothing special about which arrays are present here - they are
// simply what is needed elsewhere in the library.


typedef struct luarray_ijz {
    ludata_ijz *ijz;
    lumem mem;
} luarray_ijz;

int luarray_mkijzn(lulog *log, luarray_ijz **ijz, size_t n);
int luarray_freeijz(luarray_ijz **ijz, int prev_status);
int luarray_reserveijz(lulog *log, luarray_ijz *ijz, size_t n);
int luarray_pushijz(lulog *log, luarray_ijz *ijz, int i, int j, double z);
size_t luarray_sizeijz(luarray_ijz *ijz);

typedef struct luarray_xyz {
    ludata_xyz *xyz;
    lumem mem;
} luarray_xyz;

int luarray_mkxyzn(lulog *log, luarray_xyz **xyz, size_t n);
int luarray_freexyz(luarray_xyz **xyz, int prev_status);
int luarray_reservexyz(lulog *log, luarray_xyz *xyz, size_t n);
int luarray_pushxyz(lulog *log, luarray_xyz *xyz, double x, double y, double z);
size_t luarray_sizexyz(luarray_xyz *xyz);

typedef struct luarray_fxyzw {
    ludata_fxyzw *fxyzw;
    lumem mem;
} luarray_fxyzw;

int luarray_mkfxyzwn(lulog *log, luarray_fxyzw **fxyzw, size_t n);
int luarray_freefxyzw(luarray_fxyzw **fxyzw, int prev_status);
int luarray_reservefxyzw(lulog *log, luarray_fxyzw *fxyzw, size_t n);
int luarray_pushfxyzw(lulog *log, luarray_fxyzw *fxyzw, double x, double y, double z, double w);
size_t luarray_sizefxyzw(luarray_fxyzw *fxyzw);
int luarray_dumpfxyzw(lulog *log, luarray_fxyzw *fxyzw, const char *name, size_t nlines);

typedef struct luarray_int {
    int *i;
    lumem mem;
} luarray_int;

int luarray_mkintn(lulog *log, luarray_int **i, size_t n);
int luarray_freeint(luarray_int **xy, int prev_status);
int luarray_reserveint(lulog *log, luarray_int *i, size_t n);
int luarray_pushint(lulog *log, luarray_int *i, int j);
size_t luarray_sizeint(luarray_int *i);

typedef struct luarray_uint {
    unsigned int *i;
    lumem mem;
} luarray_uint;

int luarray_mkuintn(lulog *log, luarray_uint **i, size_t n);
int luarray_freeuint(luarray_uint **xy, int prev_status);
int luarray_reserveuint(lulog *log, luarray_uint *i, size_t n);
int luarray_pushuint(lulog *log, luarray_uint *i, unsigned int j);
size_t luarray_sizeuint(luarray_uint *i);
int luarray_dumpuint(lulog *log, luarray_uint *i, const char *name, size_t nlines);

#endif
