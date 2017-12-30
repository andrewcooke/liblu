
#include "lu/data/int32.h"
#include "lu/array_macros.h"

LUARY_MKBASE(int32, int32_t, i)

int luary_pushint32(lulog *log, luary_int32 *i, int32_t j) {
    int status = LU_OK;
    try(luary_reserveint32(log, i, 1))
    i->i[i->mem.used++] = j;
    finally:return status;
}

LUARY_MKDUMP(luary_dumpint32, luary_int32, "%d", ptr->i[i])
