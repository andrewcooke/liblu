
#include <math.h>

#include "lustatus.h"
#include "lutriplex.h"

// a re-implementation of perlin's simplex noise that:
// - uses coordinates on a triangular grid (NOT aligned with the triangular
//   grid implicit in the lusimplex 2D code)
// - supports patterns that tile (on whole triangles)
// - has configurable noise (permutation)
// - supports only 2D
// - has configurable noise vectors
//   see http://stackoverflow.com/a/21568753/181772
// - is not intended to be as efficient as lusimplex

// this code is based on the code in lusimplex (see credits there)

// the triangular mesh coordinates are not orthogonal.
// i arbitrarily pick the following two (of three) coordinates:
// - p is along the x axis
// - q is rotated 60 degrees anti-clock from p


int lutriplex_free_config2(lutriplex_config2 **config, int prev_status) {
    if (*config) {
        free((*config)->grad);
        free((*config)->perm);
    }
    free(*config);
    *config = NULL;
    return prev_status;
}

int lutriplex_mkconfig2(lulog *log, lurand *rand, lutriplex_config2 **config,
        int n_grad, double phase, int n_perm) {
    int i;
    LU_STATUS
    LU_ALLOC(log, *config, 1)
    LU_ALLOC(log, (*config)->grad, n_grad)
    LU_ALLOC(log, (*config)->perm, n_perm * 2)
    (*config)->n_grad = n_grad;
    (*config)->n_perm = n_perm;
    for (i = 0; i < n_grad; ++i) {
        double theta = phase + i*2*M_PI / n_grad;
        lutriplex_grad2 grad = {cos(theta), sin(theta)};
        (*config)->grad[i] = grad;
    }
    for (i = 0; i < n_perm; ++i) (*config)->perm[i] = i;
    LU_CHECK(lurand_shuffle(log, rand, (*config)->perm, sizeof(*(*config)->perm), n_perm))
    for (i = 0; i < n_perm; ++i) (*config)->perm[i+n_perm] = (*config)->perm[i];
    LU_NO_CLEANUP
}

int lutriplex_default_config2(lulog *log, lutriplex_config2 **config) {
    lurand *rand = NULL;
    LU_STATUS
    LU_CHECK(lurand_mkxoroshiro128plus(log, &rand, 0));
    LU_CHECK(lutriplex_mkconfig2(log, rand, config, 9, 0, 256))
LU_CLEANUP
    if (rand) status = rand->free(&rand, status);
    LU_RETURN
}


#define COS60 0.5
#define SIN60 (sqrt(3)/2)

static inline double dot2(lutriplex_grad2 g, double x, double y) {
    return g.x * x + g.y * y;
}

static inline double scale2(double dx, double dy, lutriplex_grad2 g) {
    double t = 0.5 - dx*dx - dy*dy;
    if (t < 0) {
        return 0;
    } else {
        t *= t;
        return t * t * dot2(g, dx, dy);
    }
}

int lutriplex_noise2(lulog *log, lutriplex_config2 *conf,
        double pin, double qin, double *noise) {
    LU_STATUS
    int pi = floor(pin), qi = floor(qin);
    double p = pin - pi, q = qin - qi;
    // near or far triangle in the rhombus
    int far = (p + q) > 1;
    double x = p + q * COS60, y = q * SIN60;
    double x0 = 1, y0 = 0;
    double x1 = COS60, y1 = SIN60;
    double x2 = far ? 1 + COS60 : 0, y2 = far ? SIN60 : 0;
    double dx0 = x - x0, dy0 = y - y0;
    double dx1 = x - x1, dy1 = y - y1;
    double dx2 = x - x2, dy1 = y - y2;
    int pmod = pi % conf->n_perm, qmod = qi % conf->n_perm;
    lutriplex_grad2 g0 = conf->grad[conf->perm[pmod + conf->perm[pmod]]];
    lutriplex_grad2 g1 = conf->grad[conf->perm[qmod + conf->perm[qmod]]];
    lutriplex_grad2 g2 = conf->grad[conf->perm[far + pmod + conf->perm[far + qmod]]];
    *noise = (scale2(g0, dx0, dy0) + scale2(g1, dx1, dy1) + scale2(g1, dx1, dy1));
    LU_NO_CLEANUP
}

