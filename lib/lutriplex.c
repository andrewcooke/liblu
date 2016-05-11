
#include <string.h>
#include <math.h>

#include "lustatus.h"
#include "lutriplex.h"


int lutriplex_free_config(lutriplex_config **config, int prev_status) {
    if (*config) {
        free((*config)->grad);
        free((*config)->perm);
    }
    free(*config);
    *config = NULL;
    return prev_status;
}

int lutriplex_mkconfig(lulog *log, lurand *rand, lutriplex_config **config,
        size_t n_grad, double phase, size_t n_perm) {
    size_t i;
    LU_STATUS
    LU_ALLOC(log, *config, 1)
    LU_ALLOC(log, (*config)->grad, n_grad)
    LU_ALLOC(log, (*config)->perm, n_perm * 2)
    (*config)->n_grad = n_grad;
    (*config)->n_perm = n_perm;
    for (i = 0; i < n_grad; ++i) {
        double theta = phase + i*2*M_PI / n_grad;
        ludata_xy grad = {cos(theta), sin(theta)};
        (*config)->grad[i] = grad;
    }
    for (i = 0; i < n_perm; ++i) (*config)->perm[i] = i;
    LU_CHECK(lurand_shuffle(log, rand, (*config)->perm, sizeof(*(*config)->perm), n_perm))
    for (i = 0; i < n_perm; ++i) (*config)->perm[i+n_perm] = (*config)->perm[i];
    LU_NO_CLEANUP
}

int lutriplex_default_config(lulog *log, lutriplex_config **config) {
    lurand *rand = NULL;
    LU_STATUS
    LU_CHECK(lurand_mkxoroshiro128plus(log, &rand, 0));
    LU_CHECK(lutriplex_mkconfig(log, rand, config, 9, 0, 256))
LU_CLEANUP
    if (rand) status = rand->free(&rand, status);
    LU_RETURN
}

static inline double dot(ludata_xy g, double x, double y) {
    return g.x * x + g.y * y;
}

static inline double scale(ludata_xy g, double dx, double dy) {
    double t = 0.5 - dx*dx - dy*dy;
    if (t < 0) {
        return 0;
    } else {
        t *= t;
        return t * t * dot(g, dx, dy);
    }
}

int lutriplex_noise(lulog *log, lutriplex_config *conf,
        double pin, double qin, double *noise) {
    LU_STATUS
    double cos60 = 0.5, sin60 = sqrt(3)/2;
    size_t pi = floor(pin), qi = floor(qin);
    double p = pin - pi, q = qin - qi;
    // near or far triangle in the rhombus
    size_t far = (p + q) > 1;
    double x = p + q * cos60, y = q * sin60;
    double x0 = 1, y0 = 0;
    double x1 = cos60, y1 = sin60;
    double x2 = far ? 1 + cos60 : 0, y2 = far ? sin60 : 0;
    double dx0 = x - x0, dy0 = y - y0;
    double dx1 = x - x1, dy1 = y - y1;
    double dx2 = x - x2, dy2 = y - y2;
    size_t pmod = pi % conf->n_perm, qmod = qi % conf->n_perm;
    ludata_xy g0 = conf->grad[conf->perm[pmod + conf->perm[pmod]]];
    ludata_xy g1 = conf->grad[conf->perm[qmod + conf->perm[qmod]]];
    ludata_xy g2 = conf->grad[conf->perm[far + pmod + conf->perm[far + qmod]]];
    *noise = (scale(g0, dx0, dy0) + scale(g1, dx1, dy1) + scale(g2, dx2, dy2));
    LU_NO_CLEANUP
}



int tri_free(struct lutriplex_tile **tile, size_t prev_status) {
    if (*tile) free((*tile)->state);
    return prev_status;
}

int tri_enumerate(struct lutriplex_tile *tile, lulog *log, lutriplex_config *config,
        ludata_ij corner0, uint edges, luarray_ijz **ijz) {
    LU_STATUS
    size_t i, j, points = 1 + tile->side * tile->subsamples;
    double z;
    if (!*ijz) {
        LU_CHECK(luarray_mkijzn(log, ijz, points * (points - 1)))
    }
    for (i = 0; i < points; ++i) {
        if ((i == 0 && (edges & 1)) || i) {
            double q = ((double)i) / tile->subsamples;
            for (j = 0; j < points - i; ++j) {
                if ((j == points - 1 && (edges & 2)) || (j > 0 && j < i-1) || (j == 0 && (edges & 4))) {
                    double p = ((double)j) / tile->subsamples;
                    LU_CHECK(lutriplex_noise(log, config, p, q, &z));
                    LU_CHECK(luarray_pushijz(log, *ijz, i, j, z));
                }
            }
        }
    }
    LU_NO_CLEANUP
}

int lutriplex_mktriangle(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples) {
    LU_STATUS
    LU_ASSERT(side > 0, "Side must be non-zero", LU_ERR_ARG);
    LU_ASSERT(subsamples > 0, "Subsamples must be non-zero", LU_ERR_ARG);
    LU_ALLOC(log, *tile, 1)
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->enumerate = tri_enumerate;
    (*tile)->free = tri_free;
    LU_NO_CLEANUP
}


int hex_free(struct lutriplex_tile **tile, size_t prev_status) {
    if (*tile) free((*tile)->state);
    return prev_status;
}

int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples) {
    LU_STATUS
    LU_ASSERT(side > 0, "Side must be non-zero", LU_ERR_ARG);
    LU_ASSERT(subsamples > 0, "Subsamples must be non-zero", LU_ERR_ARG);
    LU_ALLOC(log, *tile, 1)
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->enumerate = NULL;
    (*tile)->free = hex_free;
    LU_NO_CLEANUP
}


