
#ifndef LU_ARRAY_H
#define LU_ARRAY_H

#include "lumem.h"
#include "ludata.h"

typedef struct luarray_ijz {
    ludata_ijz *ijz;
    lumem mem;
} luarray_ijz;

int luarray_mkijzn(lulog *log, luarray_ijz **ijz, int n);
int luarray_freeijz(luarray_ijz **xy, int prev_status);
int luarray_reserveijz(lulog *log, luarray_ijz *ijz, int n);
int luarray_pushijz(lulog *log, luarray_ijz *ijz, int i, int j, double z);

#endif
