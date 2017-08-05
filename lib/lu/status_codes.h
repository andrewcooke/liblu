
#ifndef LU_STATUS_CODES_H
#define LU_STATUS_CODES_H

/**
 * @file
 *
 * These are the status codes returned by (almost) all functions in
 * this library (exceptions are generally void and "cannot" fail).
 */
/// @{

/// No error - intentionally false.
#define LU_OK 0

/// An internal error (typically unexpected / inconsistent).
#define LU_ERR 1

/// Memory error (eg failed allocation).
#define LU_ERR_MEM 2

/// IO error (eg read error).
#define LU_ERR_IO 3

/// Argument error (eg bad input from user).
#define LU_ERR_ARG 4

/// Matrix error (eg cannot invert).
#define LU_ERR_MAT 5

/// @}
#endif
