
#include <lu/arrays.h>
#include "lu/data/ijz.h"

LUARY_MKBASE(ijz, ludta_ijz, ijz)

int luary_ijz_push(lulog *log, luary_ijz *ijz, int32_t i, int32_t j, double z) {
    int status = LU_OK;
    try(luary_ijz_res(log, ijz, 1))
    ijz->ijz[ijz->mem.used++] = (ludta_ijz){i, j, z};
    finally:
	return status;
}

LUARY_MKDUMP(luary_ijz_dump, luary_ijz, "{%d,%d,%g}", ptr->ijz[i].i, ptr->ijz[i].j, ptr->ijz[i].z)
