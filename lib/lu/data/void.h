
#ifndef LU_DATA_VOID_H
#define LU_DATA_VOID_H

#include "../log.h"
#include "../dynamic_memory.h"

typedef struct luary_void {
    void **ptr;
    lumem mem;
} luary_void;

int luary_mkvoid(lulog *log, luary_void **ptrs, size_t n);
int luary_freevoid(luary_void **ptrs, int prev_status);
int luary_reservevoid(lulog *log, luary_void *ptrs, size_t n);
int luary_pushvoid(lulog *log, luary_void *ptrs, void *ptr);
size_t luary_sizevoid(luary_void *ptrs);
int luary_dumpvoid(lulog *log, luary_void *ptrs, const char *name, size_t nlines);

#endif
