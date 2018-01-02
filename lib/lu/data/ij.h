
#ifndef LU_DATA_IJ_H
#define LU_DATA_IJ_H

#include <stdint.h>


/**
 * @file
 *
 * @brief An (i,j) tuple.
 */
/// @{

/// An (i,j) tuple - typically a 2D coordinate.
typedef struct ludta_ij {
    int32_t i;
    int32_t j;
} ludta_ij;

/// @}
#endif
