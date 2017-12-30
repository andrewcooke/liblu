
#ifndef LU_ARRAYS_H
#define LU_ARRAYS_H

#include <stdint.h>

#include "dynamic_memory.h"
#include "gl/vectors.h"
#include "structs.h"


// there's nothing special about which arrays are present here - they are
// simply what is needed elsewhere in the library.


//typedef struct luary_xyz {
//    ludta_xyz *xyz;
//    lumem mem;
//} luary_xyz;
//
//int luary_mkxyz(lulog *log, luary_xyz **xyz, size_t n);
//int luary_freexyz(luary_xyz **xyz, int prev_status);
//int luary_reservexyz(lulog *log, luary_xyz *xyz, size_t n);
//int luary_pushxyz(lulog *log, luary_xyz *xyz, double x, double y, double z);
//size_t luary_sizexyz(luary_xyz *xyz);
//int luary_dumpxyz(lulog *log, luary_xyz *xyz, const char *name, size_t nlines);

#endif
