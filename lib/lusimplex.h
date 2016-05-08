
#ifndef LU_SIMPLEX_H
#define LU_SIMPLEX_H

double lusimplex_noise2(double xin, double yin);
double lusimplex_noise3(double xin, double yin, double zin);
double lusimplex_noise4(double xin, double yin, double zin, double win);

// for testing
void lusimplex_constants(int *n_p, uint8_t ** const p,
        double *f2, double *g2, double *f4, double *g4);

#endif
