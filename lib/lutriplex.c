
#include <string.h>
#include <math.h>

#include "lustatus.h"
#include "luminmax.h"
#include "lutriplex.h"


int lutriplex_freeconfig(lutriplex_config **config, int prev_status) {
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

int lutriplex_defaultconfig(lulog *log, lutriplex_config **config) {
    lurand *rand = NULL;
    LU_STATUS
    LU_CHECK(lurand_mkxoroshiro128plus(log, &rand, 0));
    LU_CHECK(lutriplex_mkconfig(log, rand, config, 12, 0, 256))
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

int lutriplex_noise(lulog *log, lutriplex_config *conf, lutriplex_tile *tile,
        double pin, double qin, double *noise) {
    LU_STATUS
    double cos60 = 0.5, sin60 = sqrt(3)/2;
    int pi = floor(pin), qi = floor(qin);
    double p = pin - pi, q = qin - qi;
    // near or far triangle in the rhombus
    int far = (p + q) > 1;
    double x = p + q * cos60, y = q * sin60;
    double x0 = 1, y0 = 0;
    double x1 = cos60, y1 = sin60;
    double x2 = far ? 1 + cos60 : 0, y2 = far ? sin60 : 0;
    double dx0 = x - x0, dy0 = y - y0;
    double dx1 = x - x1, dy1 = y - y1;
    double dx2 = x - x2, dy2 = y - y2;
    if (tile) tile->wrap(tile, log, &pi, &qi, &far);
    size_t pmod = pi % conf->n_perm, qmod = qi % conf->n_perm;
    // lookup below depends on the (p,q) coordinate of the corner
    // g0 is at (pi+1,qi)
    ludata_xy g0 = conf->grad[conf->perm[pmod+1+conf->perm[qmod]] % conf->n_grad];
    // g1 is at (pi,qi+1)
    ludata_xy g1 = conf->grad[conf->perm[pmod+conf->perm[qmod+1]] % conf->n_grad];
    // g2 is at (pi+far,qi+far)
    ludata_xy g2 = conf->grad[conf->perm[pmod+far+conf->perm[qmod+far]] % conf->n_grad];
    *noise = (scale(g0, dx0, dy0) + scale(g1, dx1, dy1) + scale(g2, dx2, dy2));
    LU_NO_CLEANUP
}


int generic_free(lutriplex_tile **tile, size_t prev_status) {
    if (*tile) free((*tile)->state);
    free(*tile); *tile = NULL;
    return prev_status;
}


typedef struct tri_state {
    int warn;
} tri_state;

int tri_wrap(lutriplex_tile *tile, lulog *log, int *p, int *q, int *far) {
    LU_STATUS
    tri_state *state = (tri_state*)(tile->state);
    if (!state->warn) {
        luwarn(log, "Triangle cannot be tiled - passing coords through");
        state->warn = 1;
    }
    LU_NO_CLEANUP
}

int tri_enumerate(lutriplex_tile *tile, lulog *log, lutriplex_config *config,
        ludata_ij corner0, uint edges, luarray_ijz **ijz) {
    LU_STATUS
    size_t i, j, k;
    size_t points = 1 + tile->side * tile->subsamples;
    size_t octaves = 1 + log2(tile->subsamples);
    LU_CHECK(luarray_mkijzn(log, ijz, points * (points - 1)))
    for (j = 0; j < points; ++j) {
        if ((j == 0 && (edges & 1)) || (j > 0)) {
            double q = ((double)j) / tile->subsamples;
            for (i = 0; i < points - j; ++i) {
                if ((i == points - j && (edges & 2)) || (i > 0 && i < points - j - 1) || (i == 0 && (edges & 4))) {
                    double p = ((double)i) / tile->subsamples;
                    double z = 0, dz;
                    for (k = 0; k < octaves; ++k) {
                        double a = k * points, m = pow(2, k);
                        LU_CHECK(lutriplex_noise(log, config, tile, m * p + a, m * q + a, &dz))
                        z += dz / m;
                    }
                    LU_CHECK(luarray_pushijz(log, *ijz, i + corner0.i, j + corner0.j, z))
                }
            }
        }
    }
    LU_NO_CLEANUP
}

int lutriplex_mktriangle(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples) {
    LU_STATUS
    LU_ASSERT(side > 0, log, "Side must be non-zero", LU_ERR_ARG)
    LU_ASSERT(subsamples > 0, log, "Subsamples must be non-zero", LU_ERR_ARG)
    LU_ALLOC(log, *tile, 1)
    LU_ALLOC_TYPE(log, (*tile)->state, 1, tri_state);
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->enumerate = tri_enumerate;
    (*tile)->wrap = tri_wrap;
    (*tile)->free = generic_free;
    LU_NO_CLEANUP
}


int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples) {
    LU_STATUS
    LU_ASSERT(side > 0, log, "Side must be non-zero", LU_ERR_ARG)
    LU_ASSERT(subsamples > 0, log, "Subsamples must be non-zero", LU_ERR_ARG)
    LU_ALLOC(log, *tile, 1)
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->enumerate = NULL;
    (*tile)->free = generic_free;
    LU_NO_CLEANUP
}


// y is inverted here as we're going from (p,q) to raster (top down)
static inline ludata_ij tri2raster(ludata_ijz tri) {
    return (ludata_ij){2 * tri.i + tri.j, -tri.j};
}

int lutriplex_rasterize(lulog *log, luarray_ijz *ijz, size_t *nx, size_t *ny, double **data) {
    LU_STATUS
    *nx = 0; *ny = 0; *data = NULL;
    if (!ijz->mem.used) goto exit;
    ludata_ij bl = tri2raster(ijz->ijz[0]), tr = bl;
    for (size_t i = 1; i < ijz->mem.used; ++i) {
        ludata_ij ij = tri2raster(ijz->ijz[i]);
        bl.i = min(bl.i, ij.i); bl.j = min(bl.j, ij.j);
        tr.i = max(tr.i, ij.i); tr.j = max(tr.j, ij.j);
    }
    ludebug(log, "Raster extends from (%d, %d) bottom left to (%d, %d) top right",
            bl.i, bl.j, tr.i, tr.j);
    *nx = tr.i - bl.i + 1; *ny = tr.j - bl.j + 1;
    ludebug(log, "Allocating raster area %zu x %zu", *nx, *ny);
    LU_ALLOC(log, *data, *nx * *ny)
    for (size_t i = 0; i < ijz->mem.used; ++i) {
        ludata_ij ij = tri2raster(ijz->ijz[i]);
        ij.i -= bl.i; ij.j -= bl.j;
        (*data)[ij.i + ij.j * *nx] = ijz->ijz[i].z;
    }
    int even = (ijz->ijz[0].i + ijz->ijz[0].j) % 2;
    for (size_t j = 0; j < *ny; ++j) {
        for (size_t i = (j % 2) + even; i < *nx - 2; i += 2) {
            (*data)[i + 1 + j * *nx] = 0.5 * ((*data)[i + 0 + j * *nx] + (*data)[i + 2 + j * *nx]);
        }
    }
    LU_NO_CLEANUP
}

