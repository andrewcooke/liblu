
#ifndef LU_GREY_H
#define LU_GREY_H

#include "lustr.h"

int lugrey_str(lulog *log, int *data, int nx, int ny, const char *scale, lustr *out);
int lugrey_scale(lulog *log, double *data, int n, int levels, int **out);

#endif
