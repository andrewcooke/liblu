
#include <string.h>

#include "lulog.h"
#include "lustatus.h"
#include "luarray.h"

static LUMEM_MKFREE(xy_free, ludata_xy)
static LUMEM_MKRESERVE(xy_reserve, ludata_xy)

int luarray_reservexy(lulog *log, luarray_xy *xy, int n) {
    return xy_reserve(log, &xy->xy, &xy->mem, n);
}

int luarray_mkxyn(lulog *log, luarray_xy **xy, int n) {
    LU_STATUS
    LU_ALLOC(log, *xy, 1)
    LU_CHECK(luarray_reservexy(log, *xy, n))
    LU_NO_CLEANUP
}

int luarray_freexy(luarray_xy **xy, int prev_status) {
    int status = xy_free(&(*xy)->xy, &(*xy)->mem, prev_status);
    free(*xy); *xy = NULL;
    return status;
}

int luarray_pushxy(lulog *log, luarray_xy *xy, double x, double y) {
    LU_STATUS
    LU_CHECK(luarray_reservexy(log, xy, 1))
    xy->xy[xy->mem.used++] = (ludata_xy){x, y};
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

