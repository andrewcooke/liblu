
#ifndef LU_DATA_IJZ_H
#define LU_DATA_IJZ_H

#include "../log.h"
#include "../dynamic_memory.h"

typedef struct ludta_ijz {
    int32_t i;
    int32_t j;
    double z;
} ludta_ijz;

typedef struct luary_ijz {
    ludta_ijz *ijz;
    lumem mem;
} luary_ijz;

int luary_mkijz(lulog *log, luary_ijz **ijz, size_t n);
int luary_freeijz(luary_ijz **ijz, int prev_status);
int luary_reserveijz(lulog *log, luary_ijz *ijz, size_t n);
int luary_pushijz(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z);
size_t luary_sizeijz(luary_ijz *ijz);
int luary_dumpijz(lulog *log, luary_ijz *ijz, const char *name, size_t nlines);

#endif
