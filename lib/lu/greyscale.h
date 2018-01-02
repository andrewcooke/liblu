
#ifndef LU_GREYSCALE_H
#define LU_GREYSCALE_H

#include "strings.h"
#include "log.h"

/**
 * @file
 *
 * @brief Convert 2D double arrays to grayscale text.
 */
/// @{

/// Write quantized levels to a string.
int lugry_str(lulog *log, int *data, size_t nx, size_t ny, const char *scale, lustr *out);
/// Quantize double values.
int lugry_quantize(lulog *log, double *data, int n, int levels, int **out);

/// @}
#endif
