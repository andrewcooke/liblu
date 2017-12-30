
#ifndef LU_GREYSCALE_H
#define LU_GREYSCALE_H

#include "strings.h"
#include "log.h"

int lugry_str(lulog *log, int *data, size_t nx, size_t ny, const char *scale, lustr *out);
int lugry_quantize(lulog *log, double *data, int n, int levels, int **out);

#endif
