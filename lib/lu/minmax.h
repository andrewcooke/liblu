
#ifndef LU_MINMAX_H
#define LU_MINMAX_H

/**
 * @file
 *
 * @brief Implementations of `min()` and `max()`
 *
 * Taken from
 * http://stackoverflow.com/questions/3437404/min-and-max-in-c
 *
 * If these cause conflicts they can be excluded by defining `LU_NOMINMAX`.
 */

#ifndef LU_NOMINMAX

/// Return maximum of two values.
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a >= _b ? _a : _b; })

/// Return minimum of two values.
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a <= _b ? _a : _b; })

#endif

#endif
