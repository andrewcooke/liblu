
#include <string.h>

#include "lulog.h"
#include "lustatus.h"
#include "luarray.h"


static LUMEM_MKFREE(ijz_free, ludata_ijz)
static LUMEM_MKRESERVE(ijz_reserve, ludata_ijz)

int luarray_reserveijz(lulog *log, luarray_ijz *ijz, int n) {
    return ijz_reserve(log, &ijz->ijz, &ijz->mem, n);
}

int luarray_mkijzn(lulog *log, luarray_ijz **ijz, int n) {
    LU_STATUS
    LU_ALLOC(log, *ijz, 1)
    LU_CHECK(luarray_reserveijz(log, *ijz, n))
    LU_NO_CLEANUP
}

int luarray_freeijz(luarray_ijz **ijz, int prev_status) {
    int status = ijz_free(&(*ijz)->ijz, &(*ijz)->mem, prev_status);
    free(*ijz); *ijz = NULL;
    return status;
}

int luarray_pushijz(lulog *log, luarray_ijz *ijz, int i, int j, double z) {
    LU_STATUS
    LU_CHECK(luarray_reserveijz(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludata_ijz){i, j, z};
    LU_NO_CLEANUP
}


static LUMEM_MKFREE(xyz_free, ludata_xyz)
static LUMEM_MKRESERVE(xyz_reserve, ludata_xyz)

int luarray_reservexyz(lulog *log, luarray_xyz *xyz, int n) {
    return xyz_reserve(log, &xyz->xyz, &xyz->mem, n);
}

int luarray_mkxyzn(lulog *log, luarray_xyz **xyz, int n) {
    LU_STATUS
    LU_ALLOC(log, *xyz, 1)
    LU_CHECK(luarray_reservexyz(log, *xyz, n))
    LU_NO_CLEANUP
}

int luarray_freexyz(luarray_xyz **xyz, int prev_status) {
    int status = xyz_free(&(*xyz)->xyz, &(*xyz)->mem, prev_status);
    free(*xyz); *xyz = NULL;
    return status;
}

int luarray_pushxyz(lulog *log, luarray_xyz *xyz, double x, double y, double z) {
    LU_STATUS
    LU_CHECK(luarray_reservexyz(log, xyz, 1))
    xyz->xyz[xyz->mem.used++] = (ludata_xyz){x, y, z};
    LU_NO_CLEANUP
}


static LUMEM_MKFREE(int_free, int)
static LUMEM_MKRESERVE(int_reserve, int)

int luarray_reserveint(lulog *log, luarray_int *i, int n) {
    return int_reserve(log, &i->i, &i->mem, n);
}

int luarray_mkintn(lulog *log, luarray_int **i, int n) {
    LU_STATUS
    LU_ALLOC(log, *i, 1)
    LU_CHECK(luarray_reserveint(log, *i, n))
    LU_NO_CLEANUP
}

int luarray_freeint(luarray_int **i, int prev_status) {
    int status = int_free(&(*i)->i, &(*i)->mem, prev_status);
    free(*i); *i = NULL;
    return status;
}

int luarray_pushint(lulog *log, luarray_int *i, int j) {
    LU_STATUS
    LU_CHECK(luarray_reserveint(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

