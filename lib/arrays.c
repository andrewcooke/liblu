
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/arrays.h"
#include "lu/array_macros.h"


LUARRAY_MKBASE(ijz, luarray_ijz, ludata_ijz, ijz)

int luarray_pushijz(lulog *log, luarray_ijz *ijz, int32_t i, int32_t j, double z) {
    LU_STATUS
    LU_CHECK(luarray_reserveijz(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludata_ijz){i, j, z};
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpijz, luarray_ijz, 5, "{%d,%d,%g}", ptr->ijz[i].i, ptr->ijz[i].j, ptr->ijz[i].z)


LUARRAY_MKBASE(xyz, luarray_xyz, ludata_xyz, xyz)

int luarray_pushxyz(lulog *log, luarray_xyz *xyz, double x, double y, double z) {
    LU_STATUS
    LU_CHECK(luarray_reservexyz(log, xyz, 1))
    xyz->xyz[xyz->mem.used++] = (ludata_xyz){x, y, z};
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpxyz, luarray_xyz, 3, "{%g,%g,%g}", ptr->xyz[i].z, ptr->xyz[i].y, ptr->xyz[i].z)


LUARRAY_MKBASE(fxyzw, luarray_fxyzw, ludata_fxyzw, fxyzw)

int luarray_pushfxyzw(lulog *log, luarray_fxyzw *fxyzw, float x, float y, float z, float w) {
    LU_STATUS
    LU_CHECK(luarray_reservefxyzw(log, fxyzw, 1))
    fxyzw->fxyzw[fxyzw->mem.used++] = (ludata_fxyzw){x, y, z, w};
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpfxyzw, luarray_fxyzw, 3, "{%g,%g,%g,%g}",
        ptr->fxyzw[i].x, ptr->fxyzw[i].y, ptr->fxyzw[i].z, ptr->fxyzw[i].w)


LUARRAY_MKBASE(int32, luarray_int32, int32_t, i)

int luarray_pushint32(lulog *log, luarray_int32 *i, int32_t j) {
    LU_STATUS
    LU_CHECK(luarray_reserveint32(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpint32, luarray_int32, 10, "%d", ptr->i[i])


LUARRAY_MKBASE(uint32, luarray_uint32, uint32_t, i)

int luarray_pushuint32(lulog *log, luarray_uint32 *i, uint32_t j) {
    LU_STATUS
    LU_CHECK(luarray_reserveuint32(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpuint32, luarray_uint32, 10, "%u", ptr->i[i])


LUARRAY_MKBASE(void, luarray_void, void *, ptr)

int luarray_pushvoid(lulog *log, luarray_void *ptrs, void *ptr) {
    LU_STATUS
    LU_CHECK(luarray_reservevoid(log, ptrs, 1))
    ptrs->ptr[ptrs->mem.used++] = ptr;
    LU_NO_CLEANUP
}

LUARRAY_MKDUMP(luarray_dumpvoid, luarray_void, 4, "%016x", (uint64_t)ptr->ptr[i])

