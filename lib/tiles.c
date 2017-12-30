
#include <string.h>
#include <math.h>

#include "lu/status.h"
#include "lu/minmax.h"
#include "lu/tiles.h"


int lutle_freeconfig(lutle_config **config, int prev_status) {
    if (*config) {
        free((*config)->grad);
        free((*config)->perm);
    }
    free(*config);
    *config = NULL;
    return prev_status;
}

int lutle_mkconfig(lulog *log, lutle_config **config, luran *rand,
        size_t n_grad, double phase, size_t n_perm) {
    size_t i;
    int status = LU_OK;
    LU_ALLOC(log, *config, 1)
    LU_ALLOC(log, (*config)->grad, n_grad)
    LU_ALLOC(log, (*config)->perm, n_perm)
    (*config)->n_grad = n_grad;
    (*config)->n_perm = n_perm;
    for (i = 0; i < n_grad; ++i) {
        double theta = phase + i*2*M_PI / n_grad;
        ludta_xy grad = {cos(theta), sin(theta)};
        (*config)->grad[i] = grad;
    }
    for (i = 0; i < n_perm; ++i) (*config)->perm[i] = i;
    try(luran_shuffle(log, rand, (*config)->perm, sizeof(*(*config)->perm), n_perm))
    exit:return status;
}

int lutle_defaultconfig(lulog *log, lutle_config **config, uint64_t seed) {
    luran *rand = NULL;
    int status = LU_OK;
    try(luran_mkxoroshiro128plus(log, &rand, seed));
    try(lutle_mkconfig(log, config, rand, 12, 0, 256))
exit:
    if (rand) status = rand->free(&rand, status);
    return status;
}


static inline double dot(ludta_xy g, double x, double y) {
    return g.x * x + g.y * y;
}

static inline double scale(ludta_xy g, double dx, double dy) {
    double t = 0.5 - dx*dx - dy*dy;
    if (t < 0) {
        return 0;
    } else {
        t *= t;
        return t * t * dot(g, dx, dy);
    }
}

static inline ludta_xy lookup_grad(lulog *log, lutle_config *conf,
        lutle_tile *tile, int pi, int qi) {
    if (tile) tile->wrap(tile, log, &pi, &qi);
    return conf->grad[conf->perm[(pi + conf->perm[qi % conf->n_perm]) % conf->n_perm] % conf->n_grad];
}

// this can be called with tile=NULL for direct evaluation.
int lutle_noise(lulog *log, lutle_config *conf, lutle_tile *tile,
        double pin, double qin, double *noise) {
    int status = LU_OK;
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
    int pmod, qmod;
    // lookup below depends on the (p,q) coordinate of the corner
    // g0 is at (pi+1,qi)
    ludta_xy g0 = lookup_grad(log, conf, tile, pi+1, qi);
    // g1 is at (pi,qi+1)
    ludta_xy g1 = lookup_grad(log, conf, tile, pi, qi+1);
    // g2 is at (pi+far,qi+far)
    ludta_xy g2 = lookup_grad(log, conf, tile, pi+far, qi+far);
    *noise = (scale(g0, dx0, dy0) + scale(g1, dx1, dy1) + scale(g2, dx2, dy2));
    exit:return status;
}


int generic_free(lutle_tile **tile, size_t prev_status) {
    if (*tile) free((*tile)->state);
    free(*tile); *tile = NULL;
    return prev_status;
}


// this block of functions describes the transform (and inverse) used when
// generating octave noise.  the inverse is necessary so that the tile
// can do the correct wrapping (without coupling the noise code).

static inline size_t octa(lutle_tile *tile) {
    return tile->octave * tile->side * tile->subsamples;
}

static inline size_t octm(lutle_tile *tile) {
    return pow(2, tile->octave);
}

static inline void octtransform(lutle_tile *tile, lulog *log,
        double p, double q, double *po, double *qo) {
    size_t a = octa(tile), m = octm(tile);
    *po = m * p + a; *qo =  m * q + a;
}

static inline void octshift(lutle_tile *tile, lulog *log,
        int p, int q, int *po, int *qo) {
    size_t a = octa(tile);
    *po = p + a; *qo = q + a;
}

static inline void octunshift(lutle_tile *tile, lulog *log,
        int po, int qo, int *p, int *q) {
    size_t a = octa(tile);
    *p = po - a; *q = qo - a;
}

static inline int octave(lutle_tile *tile, lulog *log,
        lutle_config *config, int i, int j, luary_ijz **ijz) {
    int status = LU_OK;
    double z = 0, dz, po, qo;
    double p = ((double)i) / tile->subsamples;
    double q = ((double)j) / tile->subsamples;
    for (tile->octave = 0; tile->octave < 1 + log2(tile->subsamples); ++tile->octave) {
        octtransform(tile, log, p, q, &po, &qo);
        try(lutle_noise(log, config, tile, po, qo, &dz))
        z += dz / pow(2 / tile->octweight, tile->octave);
    }
    try(luary_pushijz(log, *ijz, i, j, z))
    exit:return status;
}


typedef struct tri_state {
    int warn;
} tri_state;

void tri_wrap(lutle_tile *tile, lulog *log, int *p, int *q) {
    tri_state *state = (tri_state*)(tile->state);
    if (!state->warn) {
        luwarn(log, "Triangle cannot be tiled - passing coords through");
        state->warn = 1;
    }
}

int tri_enumerate(lutle_tile *tile, lulog *log, lutle_config *config,
        uint edges, luary_ijz **ijz) {
    int status = LU_OK;
    size_t i, j;
    size_t points = tile->side * tile->subsamples;
    try(luary_mkijz(log, ijz, points * (points - 1)))
    for (j = 0; j <= points; ++j) {
        if ((j == 0 && (edges & 1)) || (j > 0)) {
            for (i = 0; i <= points - j; ++i) {
                if ((i == points - j && (edges & 2)) ||
                        (i > 0 && i < points - j) ||
                        (i == 0 && (edges & 4))) {
                    try(octave(tile, log, config, i, j, ijz))
                }
            }
        }
    }
    exit:return status;
}

int lutle_mktriangle(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight) {
    int status = LU_OK;
    LU_ASSERT(side > 0, LU_ERR_ARG, log, "Side must be non-zero")
    LU_ASSERT(subsamples > 0, LU_ERR_ARG, log, "Subsamples must be non-zero")
    if (side * subsamples == 1) luwarn(log, "Only zero outer points visible");
    LU_ALLOC(log, *tile, 1)
    LU_ALLOC_TYPE(log, (*tile)->state, 1, tri_state);
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->octweight = octweight;
    (*tile)->enumerate = tri_enumerate;
    (*tile)->wrap = tri_wrap;
    (*tile)->free = generic_free;
    exit:return status;
}


void hex_wrap(lutle_tile *tile, lulog *log, int *po, int *qo) {
    int u, v, m = octm(tile);
    int s = tile->side * m;  // the size of the repeating octave tile
    octunshift(tile, log, *po, *qo, &u, &v);  // keep octave scaling
    if ((u == s && v == 0) || (u == -s && v == s)) {
        // corners 2 and 4 should match corner 0
        u = 0; v = s;
        octshift(tile, log, u, v, po, qo);
    } else if ((u == s && v == s) || (u == -s && v == 0)) {
        // corners 3 and 5 should match corner 1
        u = s; v = -s;
        octshift(tile, log, u, v, po, qo);
    } else if (v == s) {
        // edge 3 should match edge 0
        u += s; v = -s;
        octshift(tile, log, u, v, po, qo);
    } else if (u == -s) {
        // edge 4 should match edge 1
        u = s; v -= s;
        octshift(tile, log, u, v, po, qo);
    } else if (v - u == s) {
        // edge 5 should match edge 2
        u += s; v += s;
        octshift(tile, log, u, v, po, qo);
    }
    // otherwise, continue as before
}

int hex_enumerate(lutle_tile *tile, lulog *log, lutle_config *config,
        uint edges, luary_ijz **ijz) {
    int status = LU_OK;
    int points = tile->side * tile->subsamples;
    try(luary_mkijz(log, ijz, 6 * points * (points + 1)))
    int jlo = !(edges & 1) - points;
    int jhi = points - !(edges & 8);
    for (int j = jlo; j <= jhi; ++j) {
        int ilo, ihi;
        if (j >= 0) {
            ilo = !(edges & 16) - points;
            ihi = points - j - !(edges & 4);
        } else {
            ilo = !(edges & 32) - points - j;
            ihi = points - !(edges & 2);
        }
        for (int i = ilo; i <= ihi; ++i) {
            try(octave(tile, log, config, i, j, ijz))
        }
    }
    exit:return status;
}

int lutle_mkhexagon(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight) {
    int status = LU_OK;
    LU_ASSERT(side > 0, LU_ERR_ARG, log, "Side must be non-zero")
    LU_ASSERT(subsamples > 0, LU_ERR_ARG, log, "Subsamples must be non-zero")
    if (side * subsamples == 1) luwarn(log, "Only zero outer points visible");
    LU_ALLOC(log, *tile, 1)
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->octweight = octweight;
    (*tile)->enumerate = hex_enumerate;
    (*tile)->wrap = hex_wrap;
    (*tile)->free = generic_free;
    exit:return status;
}


// y is inverted here as we're going from (p,q) to raster (top down)
static inline ludta_ij tri2raster(ludta_ijz tri) {
    return (ludta_ij){2 * tri.i + tri.j, -tri.j};
}

int lutle_range(lulog *log, luary_ijz *ijz, ludta_ij *bl, ludta_ij *tr, double *zero) {
    int status = LU_OK;
    LU_ASSERT(ijz->mem.used, LU_ERR_ARG, log, "No data");
    *bl = tri2raster(ijz->ijz[0]), *tr = *bl;
    if (zero) *zero = ijz->ijz[0].z;
    for (size_t i = 1; i < ijz->mem.used; ++i) {
        ludta_ij ij = tri2raster(ijz->ijz[i]);
        bl->i = min(bl->i, ij.i); bl->j = min(bl->j, ij.j);
        tr->i = max(tr->i, ij.i); tr->j = max(tr->j, ij.j);
        if (zero) *zero = min(*zero, ijz->ijz[i].z);
    }
    ludebug(log, "Data extend from (%d, %d) bottom left to (%d, %d) top right",
            bl->i, bl->j, tr->i, tr->j);
    if (zero) ludebug(log, "Zero level is %.2g", *zero);
    exit:return status;
}

int lutle_rasterize(lulog *log, luary_ijz *ijz, size_t *nx, size_t *ny, double **data) {
    int status = LU_OK;
    ludta_ij bl, tr;
    double zero;
    *nx = 0; *ny = 0; *data = NULL;
    try(lutle_range(log, ijz, &bl, &tr, &zero))
    size_t border = 1;
    *nx = tr.i - bl.i + 1 + 2 * border; *ny = tr.j - bl.j + 1 + 2 * border;
    luinfo(log, "Allocating raster area %zu x %zu", *nx, *ny);
    LU_ALLOC(log, *data, *nx * *ny)
    for (size_t i = 0; i < ijz->mem.used; ++i) {
        ludta_ij ij = tri2raster(ijz->ijz[i]);
        ij.i -= bl.i; ij.j -= bl.j;
        (*data)[ij.i + border + (ij.j + border) * *nx] = ijz->ijz[i].z - zero;
    }
    int odd = (bl.i + bl.j) % 2;
    for (size_t j = 0; j < *ny; ++j) {
        for (size_t i = (j + odd) % 2; i < *nx - 2; i += 2) {
            (*data)[i + 1 + j * *nx] = 0.5 * ((*data)[i + 0 + j * *nx] + (*data)[i + 2 + j * *nx]);
        }
    }
    exit:return status;
}
