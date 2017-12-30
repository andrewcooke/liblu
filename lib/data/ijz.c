
#include "lu/data/ijz.h"
#include "lu/array_macros.h"

LUARY_MKBASE(ijz, ludta_ijz, ijz)

int luary_pushijz(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z) {
    int status = LU_OK;
    try(luary_reserveijz(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludta_ijz){i, j, z};
    finally:return status;
}

LUARY_MKDUMP(luary_dumpijz, luary_ijz, "{%d,%d,%g}", ptr->ijz[i].i, ptr->ijz[i].j, ptr->ijz[i].z)
