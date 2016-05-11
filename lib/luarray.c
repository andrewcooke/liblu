
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

