
#include <string.h>

#include "lulog.h"
#include "lustatus.h"
#include "luarray.h"
#include "luarray_mem.h"


LUARRAY_MKBASE(ijz, luarray_ijz, ludata_ijz, ijz)

int luarray_pushijz(lulog *log, luarray_ijz *ijz, int i, int j, double z) {
    LU_STATUS
    LU_CHECK(luarray_reserveijz(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludata_ijz){i, j, z};
    LU_NO_CLEANUP
}


LUARRAY_MKBASE(xyz, luarray_xyz, ludata_xyz, xyz)

int luarray_pushxyz(lulog *log, luarray_xyz *xyz, double x, double y, double z) {
    LU_STATUS
    LU_CHECK(luarray_reservexyz(log, xyz, 1))
    xyz->xyz[xyz->mem.used++] = (ludata_xyz){x, y, z};
    LU_NO_CLEANUP
}


LUARRAY_MKBASE(int, luarray_int, int, i)

int luarray_pushint(lulog *log, luarray_int *i, int j) {
    LU_STATUS
    LU_CHECK(luarray_reserveint(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}
