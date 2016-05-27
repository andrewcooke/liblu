
#ifndef LU_STRUCTS_H
#define LU_STRUCTS_H

#include <stdint.h>


typedef struct ludata_xy {
    double x;
    double y;
} ludata_xy;

typedef struct ludata_xyz {
    double x;
    double y;
    double z;
} ludata_xyz;

typedef struct ludata_fxyzw {
    float x;
    float y;
    float z;
    float w;
} ludata_fxyzw;

typedef struct ludata_ij {
    int32_t i;
    int32_t j;
} ludata_ij;

typedef struct ludata_ijz {
    int32_t i;
    int32_t j;
    double z;
} ludata_ijz;

#endif
