
#include <lu/arrays.h>
#include "lu/data/int32.h"

LUARY_MKBASE(int32, int32_t, i)

int luary_int32_push(lulog *log, luary_int32 *i, int32_t j) {
    int status = LU_OK;
    try(luary_int32_res(log, i, 1))
    i->i[i->mem.used++] = j;
    finally:
	return status;
}

LUARY_MKDUMP(luary_int32_dump, luary_int32, "%d", ptr->i[i])
