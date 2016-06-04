
#ifndef LU_ARRAYS_H
#define LU_ARRAYS_H

#include <stdint.h>

#include "dynamic_memory.h"
#include "structs.h"
#include "vectors.h"


// there's nothing special about which arrays are present here - they are
// simply what is needed elsewhere in the library.


typedef struct luary_ijz {
    ludta_ijz *ijz;
    lumem mem;
} luary_ijz;

int luary_mkijzn(lulog *log, luary_ijz **ijz, size_t n);
int luary_freeijz(luary_ijz **ijz, int prev_status);
int luary_reserveijz(lulog *log, luary_ijz *ijz, size_t n);
int luary_pushijz(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z);
size_t luary_sizeijz(luary_ijz *ijz);
int luary_dumpijz(lulog *log, luary_ijz *ijz, const char *name, size_t nlines);

typedef struct luary_xyz {
    ludta_xyz *xyz;
    lumem mem;
} luary_xyz;

int luary_mkxyzn(lulog *log, luary_xyz **xyz, size_t n);
int luary_freexyz(luary_xyz **xyz, int prev_status);
int luary_reservexyz(lulog *log, luary_xyz *xyz, size_t n);
int luary_pushxyz(lulog *log, luary_xyz *xyz, double x, double y, double z);
size_t luary_sizexyz(luary_xyz *xyz);
int luary_dumpxyz(lulog *log, luary_xyz *xyz, const char *name, size_t nlines);

typedef struct luary_fxyzw {
    ludta_fxyzw *fxyzw;
    lumem mem;
} luary_fxyzw;

int luary_mkfxyzwn(lulog *log, luary_fxyzw **fxyzw, size_t n);
int luary_freefxyzw(luary_fxyzw **fxyzw, int prev_status);
int luary_reservefxyzw(lulog *log, luary_fxyzw *fxyzw, size_t n);
int luary_pushfxyzw(lulog *log, luary_fxyzw *fxyzw, float x, float y, float z, float w);
size_t luary_sizefxyzw(luary_fxyzw *fxyzw);
int luary_dumpfxyzw(lulog *log, luary_fxyzw *fxyzw, const char *name, size_t nlines);

typedef struct luary_int32 {
    int32_t *i;
    lumem mem;
} luary_int32;

int luary_mkint32n(lulog *log, luary_int32 **i, size_t n);
int luary_freeint32(luary_int32 **i, int prev_status);
int luary_reserveint32(lulog *log, luary_int32 *i, size_t n);
int luary_pushint32(lulog *log, luary_int32 *i, int j);
size_t luary_sizeint32(luary_int32 *i);
int luary_dumpint32(lulog *log, luary_int32 *i, const char *name, size_t nlines);

typedef struct luary_uint32 {
    uint32_t *i;
    lumem mem;
} luary_uint32;

int luary_mkuint32n(lulog *log, luary_uint32 **i, size_t n);
int luary_freeuint32(luary_uint32 **i, int prev_status);
int luary_reserveuint32(lulog *log, luary_uint32 *i, size_t n);
int luary_pushuint32(lulog *log, luary_uint32 *i, unsigned int j);
size_t luary_sizeuint32(luary_uint32 *i);
int luary_dumpuint32(lulog *log, luary_uint32 *i, const char *name, size_t nlines);

typedef struct luary_void {
    void **ptr;
    lumem mem;
} luary_void;

int luary_mkvoidn(lulog *log, luary_void **ptrs, size_t n);
int luary_freevoid(luary_void **ptrs, int prev_status);
int luary_reservevoid(lulog *log, luary_void *ptrs, size_t n);
int luary_pushvoid(lulog *log, luary_void *ptrs, void *ptr);
size_t luary_sizevoid(luary_void *ptrs);
int luary_dumpvoid(lulog *log, luary_void *ptrs, const char *name, size_t nlines);

typedef struct luary_vecf4 {
    luvec_f4 *v;
    lumem mem;
} luary_vecf4;

int luary_mkvecf4n(lulog *log, luary_vecf4 **v, size_t n);
int luary_freevecf4(luary_vecf4 **v, int prev_status);
int luary_reservevecf4(lulog *log, luary_vecf4 *v, size_t n);
int luary_pushvecf4(lulog *log, luary_vecf4 *v, float x, float y, float z, float w);
size_t luary_sizevecf4(luary_vecf4 *v);

#endif
