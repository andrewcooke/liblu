
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/arrays.h"
#include "lu/array_macros.h"


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


LUARRAY_MKBASE(fxyzw, luarray_fxyzw, ludata_fxyzw, fxyzw)

int luarray_pushfxyzw(lulog *log, luarray_fxyzw *fxyzw, double x, double y, double z, double w) {
    LU_STATUS
    LU_CHECK(luarray_reservefxyzw(log, fxyzw, 1))
    fxyzw->fxyzw[fxyzw->mem.used++] = (ludata_fxyzw){x, y, z, w};
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpfxyzw, luarray_fxyzw, 3, "{%g,%g,%g,%g}",
        ptr->fxyzw[i].x, ptr->fxyzw[i].y, ptr->fxyzw[i].z, ptr->fxyzw[i].w)


LUARRAY_MKBASE(int, luarray_int, int, i)

int luarray_pushint(lulog *log, luarray_int *i, int j) {
    LU_STATUS
    LU_CHECK(luarray_reserveint(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}


LUARRAY_MKBASE(uint, luarray_uint, unsigned int, i)

int luarray_pushuint(lulog *log, luarray_uint *i, unsigned int j) {
    LU_STATUS
    LU_CHECK(luarray_reserveuint(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpuint, luarray_uint, 20, "%u", ptr->i[i])


