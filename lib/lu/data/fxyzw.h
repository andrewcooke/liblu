
#ifndef LU_DATA_FXYZW_H
#define LU_DATA_FXYZW_H

#include "../log.h"
#include "../dynamic_memory.h"

typedef struct ludta_fxyzw {
    float x;
    float y;
    float z;
    float w;
} ludta_fxyzw;

typedef struct luary_fxyzw {
    ludta_fxyzw *fxyzw;
    lumem mem;
} luary_fxyzw;

int luary_mkfxyzw(lulog *log, luary_fxyzw **fxyzw, size_t n);
int luary_freefxyzw(luary_fxyzw **fxyzw, int prev_status);
int luary_reservefxyzw(lulog *log, luary_fxyzw *fxyzw, size_t n);
int luary_pushfxyzw(lulog *log, luary_fxyzw *fxyzw, float x, float y, float z, float w);
size_t luary_sizefxyzw(luary_fxyzw *fxyzw);
int luary_dumpfxyzw(lulog *log, luary_fxyzw *fxyzw, const char *name, size_t nlines);

char *ludta_fxyzw2str(ludta_fxyzw xyzw, int n, char *buffer);

#endif
