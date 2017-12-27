
#ifndef LU_COLOURS_H
#define LU_COLOURS_H

#include "../log.h"
#include "types.h"

/**
 * @file
 *
 * @brief Colour operations to support OpenGL programs.
 *
 * All input and output arrays passed by pointer.
 */
/// @{

/// Copy a colour (from a to b).
void luglc_copy(luglc *a, luglc *b);

/// @}
#endif
