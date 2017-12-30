
#include <lu/arrays.h>
#include <stdint.h>

#include "lu/data/void.h"

LUARY_MKBASE(void, void *, ptr)

int luary_pushvoid(lulog *log, luary_void *ptrs, void *ptr) {
    int status = LU_OK;
    try(luary_reservevoid(log, ptrs, 1))
    ptrs->ptr[ptrs->mem.used++] = ptr;
    finally:return status;
}

LUARY_MKDUMP(luary_dumpvoid, luary_void, "%016x", (uint64_t)ptr->ptr[i])
