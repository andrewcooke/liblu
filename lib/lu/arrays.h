
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

int luarray_mkijzn(const lulog *log, luarray_ijz **ijz, int n);
int luarray_freeijz(luarray_ijz **ijz, int prev_status);
int luarray_reserveijz(const lulog *log, luarray_ijz *ijz, int n);
int luarray_pushijz(const lulog *log, luarray_ijz *ijz, int i, int j, double z);

typedef struct luarray_xyz {
    ludata_xyz *xyz;
    lumem mem;
} luarray_xyz;

int luarray_mkxyzn(const lulog *log, luarray_xyz **xyz, int n);
int luarray_freexyz(luarray_xyz **xyz, int prev_status);
int luarray_reservexyz(const lulog *log, luarray_xyz *xyz, int n);
int luarray_pushxyz(const lulog *log, luarray_xyz *xyz, double x, double y, double z);


typedef struct luarray_int {
    int *i;
    lumem mem;
} luarray_int;

int luarray_mkintn(const lulog *log, luarray_int **i, int n);
int luarray_freeint(luarray_int **xy, int prev_status);
int luarray_reserveint(const lulog *log, luarray_int *i, int n);
int luarray_pushint(const lulog *log, luarray_int *i, int j);

#endif
