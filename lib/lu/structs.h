
#ifndef LU_STRUCTS_H
#define LU_STRUCTS_H

#include <stdint.h>


typedef struct ludta_xy {
    double x;
    double y;
} ludta_xy;

typedef struct ludta_xyz {
    double x;
    double y;
    double z;
} ludta_xyz;

typedef struct ludta_fxyzw {
    float x;
    float y;
    float z;
    float w;
} ludta_fxyzw;

typedef struct ludta_ij {
    int32_t i;
    int32_t j;
} ludta_ij;

typedef struct ludta_ijz {
    int32_t i;
    int32_t j;
    double z;
} ludta_ijz;

char *ludta_fxyzw2str(ludta_fxyzw xyzw, int n, char *buffer);

#endif
