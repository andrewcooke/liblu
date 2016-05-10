
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
        lutriplex_xy grad = {cos(theta), sin(theta)};
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

static inline double dot2(lutriplex_xy g, double x, double y) {
    return g.x * x + g.y * y;
}

static inline double scale2(lutriplex_xy g, double dx, double dy) {
    double t = 0.5 - dx*dx - dy*dy;
    if (t < 0) {
        return 0;
    } else {
        t *= t;
        return t * t * dot2(g, dx, dy);
    }
}

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

int lutriplex_noise2(lulog *log, lutriplex_config *conf,
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
    int pmod = pi % conf->n_perm, qmod = qi % conf->n_perm;
    lutriplex_xy g0 = conf->grad[conf->perm[pmod + conf->perm[pmod]]];
    lutriplex_xy g1 = conf->grad[conf->perm[qmod + conf->perm[qmod]]];
    lutriplex_xy g2 = conf->grad[conf->perm[far + pmod + conf->perm[far + qmod]]];
    *noise = (scale2(g0, dx0, dy0) + scale2(g1, dx1, dy1) + scale2(g2, dx2, dy2));
    LU_NO_CLEANUP
}


static LUMEM_MKFREE(xy_free, lutriplex_xy)
static LUMEM_MKRESERVE(xy_reserve, lutriplex_xy)

int lutriplex_reservexy(lulog *log, lutriplex_xys *xys, int n) {
    return xy_reserve(log, &xys->xy, &xys->mem, n);
}

int lutriplex_mkxyn(lulog *log, lutriplex_xys **xys, int n) {
    LU_STATUS
    LU_ALLOC(log, *xys, 1)
    LU_CHECK(lutriplex_reservexy(log, *xys, n))
    LU_NO_CLEANUP
}

int lutriplex_freexy(lutriplex_xys **xys, int prev_status) {
    int status = xy_free(&(*xys)->xy, &(*xys)->mem, prev_status);
    free(*xys); *xys = NULL;
    return status;
}

int lutriplex_pushxy(lulog *log, lutriplex_xys *xys, double x, double y) {
    LU_STATUS
    LU_CHECK(lutriplex_reservexy(log, xys, 1))
    xys->xy[xys->mem.used++] = (lutriplex_xy) {x, y};
    LU_NO_CLEANUP
}

static LUMEM_MKFREE(xyz_free, lutriplex_xyz)
static LUMEM_MKRESERVE(xyz_reserve, lutriplex_xyz)

int lutriplex_reservexyz(lulog *log, lutriplex_xyzs *xyzs, int n) {
    return xyz_reserve(log, &xyzs->xyz, &xyzs->mem, n);
}

int lutriplex_mkxyzn(lulog *log, lutriplex_xyzs **xyzs, int n) {
    LU_STATUS
    LU_ALLOC(log, *xyzs, 1)
    LU_CHECK(lutriplex_reservexyz(log, *xyzs, n))
    LU_NO_CLEANUP
}

int lutriplex_freexyz(lutriplex_xyzs **xyzs, int prev_status) {
    int status = xyz_free(&(*xyzs)->xyz, &(*xyzs)->mem, prev_status);
    free(*xyzs); *xyzs = NULL;
    return status;
}

int lutriplex_pushxyz(lulog *log, lutriplex_xyzs *xyzs, double x, double y, double z) {
    LU_STATUS
    LU_CHECK(lutriplex_reservexyz(log, xyzs, 1))
    xyzs->xyz[xyzs->mem.used++] = (lutriplex_xyz) {x, y, z};
    LU_NO_CLEANUP
}

