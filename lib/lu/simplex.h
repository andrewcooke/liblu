
#ifndef LU_SIMPLEX_H
#define LU_SIMPLEX_H

/**
 * @file
 *
 * @brief Perlin simplex noise reference implementation
 * (not recommended for use).
 *
 * Based on java code by Stefan Gustavson (stegu@itn.liu.se) and Peter
 * Eastman (peastman@drizzle.stanford.edu).
 * http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java
 *
 * Note that in 2d this code has some issues
 * http://stackoverflow.com/a/21568753/181772
 * and `tiles.h` provides an alternative implementation.
 */
/// @{

/// Perlin simplex noise in 2D.
double luplx_noise2(double xin, double yin);

/// Perlin simplex noise in 3D.
double luplx_noise3(double xin, double yin, double zin);

/// Perlin simplex noise in 4D.
double luplx_noise4(double xin, double yin, double zin, double win);

// for testing
void luplx_constants(int *n_p, const uint8_t **p,
        double *f2, double *g2, double *f4, double *g4);

#endif
