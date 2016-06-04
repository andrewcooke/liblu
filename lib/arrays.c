
#include <string.h>

#include "lu/log.h"
#include "lu/status.h"
#include "lu/arrays.h"
#include "lu/array_macros.h"


LUARY_MKBASE(ijz, luary_ijz, ludat_ijz, ijz)

int luary_pushijz(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z) {
    LU_STATUS
    LU_CHECK(luary_reserveijz(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludat_ijz){i, j, z};
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpijz, luary_ijz, "{%d,%d,%g}", ptr->ijz[i].i, ptr->ijz[i].j, ptr->ijz[i].z)


LUARY_MKBASE(xyz, luary_xyz, ludat_xyz, xyz)

int luary_pushxyz(lulog *log, luary_xyz *xyz, double x, double y, double z) {
    LU_STATUS
    LU_CHECK(luary_reservexyz(log, xyz, 1))
    xyz->xyz[xyz->mem.used++] = (ludat_xyz){x, y, z};
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpxyz, luary_xyz, "{%g,%g,%g}", ptr->xyz[i].z, ptr->xyz[i].y, ptr->xyz[i].z)


LUARY_MKBASE(fxyzw, luary_fxyzw, ludat_fxyzw, fxyzw)

int luary_pushfxyzw(lulog *log, luary_fxyzw *fxyzw, float x, float y, float z, float w) {
    LU_STATUS
    LU_CHECK(luary_reservefxyzw(log, fxyzw, 1))
    fxyzw->fxyzw[fxyzw->mem.used++] = (ludat_fxyzw){x, y, z, w};
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpfxyzw, luary_fxyzw, "{%g,%g,%g,%g}",
        ptr->fxyzw[i].x, ptr->fxyzw[i].y, ptr->fxyzw[i].z, ptr->fxyzw[i].w)


LUARY_MKBASE(int32, luary_int32, int32_t, i)

int luary_pushint32(lulog *log, luary_int32 *i, int32_t j) {
    LU_STATUS
    LU_CHECK(luary_reserveint32(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpint32, luary_int32, "%d", ptr->i[i])


LUARY_MKBASE(uint32, luary_uint32, uint32_t, i)

int luary_pushuint32(lulog *log, luary_uint32 *i, uint32_t j) {
    LU_STATUS
    LU_CHECK(luary_reserveuint32(log, i, 1))
    i->i[i->mem.used++] = j;
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpuint32, luary_uint32, "%u", ptr->i[i])


LUARY_MKBASE(void, luary_void, void *, ptr)

int luary_pushvoid(lulog *log, luary_void *ptrs, void *ptr) {
    LU_STATUS
    LU_CHECK(luary_reservevoid(log, ptrs, 1))
    ptrs->ptr[ptrs->mem.used++] = ptr;
    LU_NO_CLEANUP
}

LUARY_MKDUMP(luary_dumpvoid, luary_void, "%016x", (uint64_t)ptr->ptr[i])

