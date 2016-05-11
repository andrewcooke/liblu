
#ifndef LU_DATA_H
#define LU_DATA_H

typedef struct ludata_xy {
    double x;
    double y;
} ludata_xy;

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
