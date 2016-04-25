
#ifndef LU_MEM_H
#define LU_MEM_H

#include "lu.h"

#define LU_ALLOC(ptr, n) if (!(ptr = calloc(n, sizeof(*ptr)))) {status = LU_ERR_MEM; goto exit;}
#define LU_ALLOC_TYPE(ptr, n, type) if (!(ptr = calloc(n, sizeof(type)))) {status = LU_ERR_MEM; goto exit;}

#endif
