
#ifndef LU_STRUCTS_H
#define LU_STRUCTS_H

typedef struct ludata_xy {
    double x;
    double y;
} ludata_xy;

typedef struct ludata_xyz {
    double x;
    double y;
    double z;
} ludata_xyz;

typedef struct ludata_ij {
    int i;
    int j;
} ludata_ij;

typedef struct ludata_ijz {
    int i;
    int j;
    double z;
} ludata_ijz;

#endif
