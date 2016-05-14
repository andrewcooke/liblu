
#ifndef LU_GREYSCALE_H
#define LU_GREYSCALE_H

#include "strings.h"
#include "arrays.h"

int lugrey_str(lulog *log, int *data, size_t nx, size_t ny, const char *scale, lustr *out);
int lugrey_quantize(lulog *log, double *data, int n, int levels, int **out);

#endif
