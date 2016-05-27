
#ifndef LU_ARRAYS_H
#define LU_ARRAYS_H

#include <stdint.h>

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
int luarray_pushijz(lulog *log, luarray_ijz *ijz, int32_t i, int32_t j, double z);
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
int luarray_pushfxyzw(lulog *log, luarray_fxyzw *fxyzw, float x, float y, float z, float w);
size_t luarray_sizefxyzw(luarray_fxyzw *fxyzw);
int luarray_dumpfxyzw(lulog *log, luarray_fxyzw *fxyzw, const char *name, size_t nlines);

typedef struct luarray_int32 {
    int32_t *i;
    lumem mem;
} luarray_int32;

int luarray_mkint32n(lulog *log, luarray_int32 **i, size_t n);
int luarray_freeint32(luarray_int32 **xy, int prev_status);
int luarray_reserveint32(lulog *log, luarray_int32 *i, size_t n);
int luarray_pushint32(lulog *log, luarray_int32 *i, int j);
size_t luarray_sizeint32(luarray_int32 *i);

typedef struct luarray_uint32 {
    uint32_t *i;
    lumem mem;
} luarray_uint32;

int luarray_mkuint32n(lulog *log, luarray_uint32 **i, size_t n);
int luarray_freeuint32(luarray_uint32 **xy, int prev_status);
int luarray_reserveuint32(lulog *log, luarray_uint32 *i, size_t n);
int luarray_pushuint32(lulog *log, luarray_uint32 *i, unsigned int j);
size_t luarray_sizeuint32(luarray_uint32 *i);
int luarray_dumpuint32(lulog *log, luarray_uint32 *i, const char *name, size_t nlines);

#endif
