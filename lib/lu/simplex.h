
#ifndef LU_SIMPLEX_H
#define LU_SIMPLEX_H

double luplx_noise2(double xin, double yin);
double luplx_noise3(double xin, double yin, double zin);
double luplx_noise4(double xin, double yin, double zin, double win);

// for testing
void luplx_constants(int *n_p, const uint8_t **p,
        double *f2, double *g2, double *f4, double *g4);

#endif
