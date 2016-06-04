
#ifndef LU_STRUCTS_H
#define LU_STRUCTS_H

#include <stdint.h>


typedef struct ludat_xy {
    double x;
    double y;
} ludat_xy;

typedef struct ludat_xyz {
    double x;
    double y;
    double z;
} ludat_xyz;

typedef struct ludat_fxyzw {
    float x;
    float y;
    float z;
    float w;
} ludat_fxyzw;

typedef struct ludat_ij {
    int32_t i;
    int32_t j;
} ludat_ij;

typedef struct ludat_ijz {
    int32_t i;
    int32_t j;
    double z;
} ludat_ijz;

char *ludat_fxyzw2str(ludat_fxyzw xyzw, int n, char *buffer);

#endif
