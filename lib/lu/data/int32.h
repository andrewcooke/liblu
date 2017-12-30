
#ifndef LU_DATA_INT32_H
#define LU_DATA_INT32_H

#include "../log.h"
#include "../dynamic_memory.h"

typedef struct luary_int32 {
    int32_t *i;
    lumem mem;
} luary_int32;

int luary_mkint32(lulog *log, luary_int32 **i, size_t n);
int luary_freeint32(luary_int32 **i, int prev_status);
int luary_reserveint32(lulog *log, luary_int32 *i, size_t n);
int luary_pushint32(lulog *log, luary_int32 *i, int j);
size_t luary_sizeint32(luary_int32 *i);
int luary_dumpint32(lulog *log, luary_int32 *i, const char *name, size_t nlines);


#endif
