
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
    LU_ALLOC(log, (*config)->perm, n_perm)
    (*config)->n_grad = n_grad;
    (*config)->n_perm = n_perm;
    for (i = 0; i < n_grad; ++i) {
        double theta = phase + i*2*M_PI / n_grad;
        ludata_xy grad = {cos(theta), sin(theta)};
        (*config)->grad[i] = grad;
    }
    for (i = 0; i < n_perm; ++i) (*config)->perm[i] = i;
    LU_CHECK(lurand_shuffle(log, rand, (*config)->perm, sizeof(*(*config)->perm), n_perm))
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

static inline ludata_xy lookup_grad(lulog *log, lutriplex_config *conf,
        lutriplex_tile *tile, int pi, int qi) {
    if (tile) tile->wrap(tile, log, &pi, &qi);
    return conf->grad[conf->perm[(pi + conf->perm[qi % conf->n_perm]) % conf->n_perm] % conf->n_grad];
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
    int pmod, qmod;
    // lookup below depends on the (p,q) coordinate of the corner
    // g0 is at (pi+1,qi)
    ludata_xy g0 = lookup_grad(log, conf, tile, pi+1, qi);
    // g1 is at (pi,qi+1)
    ludata_xy g1 = lookup_grad(log, conf, tile, pi, qi+1);
    // g2 is at (pi+far,qi+far)
    ludata_xy g2 = lookup_grad(log, conf, tile, pi+far, qi+far);
    *noise = (scale(g0, dx0, dy0) + scale(g1, dx1, dy1) + scale(g2, dx2, dy2));
    LU_NO_CLEANUP
}


int generic_free(lutriplex_tile **tile, size_t prev_status) {
    if (*tile) free((*tile)->state);
    free(*tile); *tile = NULL;
    return prev_status;
}


// this block of functions describes the transform (and inverse) used when
// generating octave noise.  the inverse is necessary so that the tile
// can do the correct wrapping (without coupling the triplex noise code).

static inline size_t octa(lutriplex_tile *tile) {
    return tile->octave * tile->side * tile->subsamples;
}

static inline size_t octm(lutriplex_tile *tile) {
    return pow(2, tile->octave);
}

static inline void octtransform(lutriplex_tile *tile, lulog *log,
        double p, double q, double *po, double *qo) {
    size_t a = octa(tile), m = octm(tile);
    *po = m * p + a; *qo =  m * q + a;
}

static inline void octshift(lutriplex_tile *tile, lulog *log,
        int p, int q, int *po, int *qo) {
    size_t a = octa(tile);
    *po = p + a; *qo = q + a;
}

static inline void octunshift(lutriplex_tile *tile, lulog *log,
        int po, int qo, int *p, int *q) {
    size_t a = octa(tile);
    *p = po - a; *q = qo - a;
}

static inline int octave(lutriplex_tile *tile, lulog *log,
        lutriplex_config *config, int i, int j, luarray_ijz **ijz) {
    LU_STATUS
    double z = 0, dz, po, qo;
    double p = ((double)i) / tile->subsamples;
    double q = ((double)j) / tile->subsamples;
    for (tile->octave = 0; tile->octave < 1 + log2(tile->subsamples); ++tile->octave) {
        octtransform(tile, log, p, q, &po, &qo);
        LU_CHECK(lutriplex_noise(log, config, tile, po, qo, &dz))
        z += dz / pow(2 / tile->octweight, tile->octave);
    }
    LU_CHECK(luarray_pushijz(log, *ijz, i, j, z))
    LU_NO_CLEANUP
}


typedef struct tri_state {
    int warn;
} tri_state;

void tri_wrap(lutriplex_tile *tile, lulog *log, int *p, int *q) {
    tri_state *state = (tri_state*)(tile->state);
    if (!state->warn) {
        luwarn(log, "Triangle cannot be tiled - passing coords through");
        state->warn = 1;
    }
}

int tri_enumerate(lutriplex_tile *tile, lulog *log, lutriplex_config *config,
        uint edges, luarray_ijz **ijz) {
    LU_STATUS
    size_t i, j;
    size_t points = tile->side * tile->subsamples;
    LU_CHECK(luarray_mkijzn(log, ijz, points * (points - 1)))
    for (j = 0; j <= points; ++j) {
        if ((j == 0 && (edges & 1)) || (j > 0)) {
            for (i = 0; i <= points - j; ++i) {
                if ((i == points - j && (edges & 2)) ||
                        (i > 0 && i < points - j) ||
                        (i == 0 && (edges & 4))) {
                    LU_CHECK(octave(tile, log, config, i, j, ijz))
                }
            }
        }
    }
    LU_NO_CLEANUP
}

int lutriplex_mktriangle(lulog *log, lutriplex_tile **tile,
        size_t side, size_t subsamples, double octweight) {
    LU_STATUS
    LU_ASSERT(side > 0, LU_ERR_ARG, log, "Side must be non-zero")
    LU_ASSERT(subsamples > 0, LU_ERR_ARG, log, "Subsamples must be non-zero")
    LU_ASSERT(side * subsamples > 1, LU_ERR_ARG, log, "Only zero outer points visible")
    LU_ALLOC(log, *tile, 1)
    LU_ALLOC_TYPE(log, (*tile)->state, 1, tri_state);
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->octweight = octweight;
    (*tile)->enumerate = tri_enumerate;
    (*tile)->wrap = tri_wrap;
    (*tile)->free = generic_free;
    LU_NO_CLEANUP
}


void hex_wrap(lutriplex_tile *tile, lulog *log, int *po, int *qo) {
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

int hex_enumerate(lutriplex_tile *tile, lulog *log, lutriplex_config *config,
        uint edges, luarray_ijz **ijz) {
    LU_STATUS
    int points = tile->side * tile->subsamples;
    LU_CHECK(luarray_mkijzn(log, ijz, 6 * points * (points + 1)))
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
            LU_CHECK(octave(tile, log, config, i, j, ijz))
        }
    }
    LU_NO_CLEANUP
}

int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile,
        size_t side, size_t subsamples, double octweight) {
    LU_STATUS
    LU_ASSERT(side > 0, LU_ERR_ARG, log, "Side must be non-zero")
    LU_ASSERT(subsamples > 0, LU_ERR_ARG, log, "Subsamples must be non-zero")
    LU_ALLOC(log, *tile, 1)
    (*tile)->side = side;
    (*tile)->subsamples = subsamples;
    (*tile)->octweight = octweight;
    (*tile)->enumerate = hex_enumerate;
    (*tile)->wrap = hex_wrap;
    (*tile)->free = generic_free;
    LU_NO_CLEANUP
}


// y is inverted here as we're going from (p,q) to raster (top down)
static inline ludata_ij tri2raster(ludata_ijz tri) {
    return (ludata_ij){2 * tri.i + tri.j, -tri.j};
}

static inline int range(lulog *log, luarray_ijz *ijz,
        ludata_ij *bl, ludata_ij *tr, double *zero) {
    LU_STATUS
    LU_ASSERT(ijz->mem.used, LU_ERR_ARG, log, "No data");
    *bl = tri2raster(ijz->ijz[0]), *tr = *bl;
    if (zero) *zero = ijz->ijz[0].z;
    for (size_t i = 1; i < ijz->mem.used; ++i) {
        ludata_ij ij = tri2raster(ijz->ijz[i]);
        bl->i = min(bl->i, ij.i); bl->j = min(bl->j, ij.j);
        tr->i = max(tr->i, ij.i); tr->j = max(tr->j, ij.j);
        if (zero) *zero = min(*zero, ijz->ijz[i].z);
    }
    ludebug(log, "Data extend from (%d, %d) bottom left to (%d, %d) top right",
            bl->i, bl->j, tr->i, tr->j);
    if (zero) ludebug(log, "Zero level is %.2g", *zero);
    LU_NO_CLEANUP
}

int lutriplex_rasterize(lulog *log, luarray_ijz *ijz, size_t *nx, size_t *ny, double **data) {
    LU_STATUS
    ludata_ij bl, tr;
    double zero;
    *nx = 0; *ny = 0; *data = NULL;
    LU_CHECK(range(log, ijz, &bl, &tr, &zero))
    size_t border = 1;
    *nx = tr.i - bl.i + 1 + 2 * border; *ny = tr.j - bl.j + 1 + 2 * border;
    luinfo(log, "Allocating raster area %zu x %zu", *nx, *ny);
    LU_ALLOC(log, *data, *nx * *ny)
    for (size_t i = 0; i < ijz->mem.used; ++i) {
        ludata_ij ij = tri2raster(ijz->ijz[i]);
        ij.i -= bl.i; ij.j -= bl.j;
        (*data)[ij.i + border + (ij.j + border) * *nx] = ijz->ijz[i].z - zero;
    }
    int odd = (bl.i + bl.j) % 2;
    for (size_t j = 0; j < *ny; ++j) {
        for (size_t i = (j + odd) % 2; i < *nx - 2; i += 2) {
            (*data)[i + 1 + j * *nx] = 0.5 * ((*data)[i + 0 + j * *nx] + (*data)[i + 2 + j * *nx]);
        }
    }
    LU_NO_CLEANUP
}


static int ijeq(ludata_ij ij1, ludata_ij ij2) {
    return ij1.i == ij2.i && ij1.j == ij2.j;
}

static inline ludata_ij ijz2ij(ludata_ijz ijz) {
    return (ludata_ij){ijz.i, ijz.j};
}

static inline int ij2index(ludata_ij ij, ludata_ij bl, ludata_ij tr) {
    return ij.i - bl.i + (ij.j - bl.j) * (tr.i - bl.i + 1);
}

static inline int ijz2index(ludata_ijz ijz, ludata_ij bl, ludata_ij tr) {
    return ij2index(ijz2ij(ijz), bl, tr);
}

static int mkindex(lulog *log, luarray_ijz *ijz, ludata_ij bl, ludata_ij tr,
        ludata_ijz ***index) {
    LU_STATUS
    size_t nx = tr.i - bl.i + 1, ny = tr.j - bl.j + 1;
    LU_ALLOC(log, *index, nx*ny)
    for (size_t i = 0; i < ijz->mem.used; ++i) {
        (*index)[ijz2index(ijz->ijz[i], bl, tr)] = &ijz->ijz[i];
    }
    LU_NO_CLEANUP
}

static inline ludata_ij upleft(ludata_ijz p) {
    return (ludata_ij){p.i-1, p.j+1};
}

static inline ludata_ij upright(ludata_ijz p) {
    return (ludata_ij){p.i, p.j+1};
}

static inline ludata_ij right(ludata_ijz p) {
    return (ludata_ij){p.i+1, p.j};
}

static inline ludata_ij downright(ludata_ijz p) {
    return (ludata_ij){p.i+1, p.j-1};
}

static int addpoints(lulog *log, luarray_ijz *ijz, size_t *current,
        ludata_ijz *pprev, int nextisup,
        ludata_ijz **index, ludata_ij bl, ludata_ij tr,
        luarray_xyz *xyz, luarray_int *offsets) {
    LU_STATUS
    do {
        ludata_ijz *pnext = index[ij2index((nextisup ? upright : downright)(*pprev), bl, tr)];
        if (pnext) {
            if (!nextisup) {
                // we're walking geometrically (in i,j space) across the
                // points, but we need to keep the traversal of ijz (via
                // the current index) in step so we can restart correctly
                // for the next strip.  this is only possible if the points
                // are sorted (which enumerate should guarantee).
                LU_ASSERT(ijeq(right(ijz->ijz[*current]), ijz2ij(ijz->ijz[*current+1])), LU_ERR,
                        log, "Unsorted points?  Current at (%d,%d), next at (%d,%d), prev at (%d,%d), downright at (%d,%d)",
                        ijz->ijz[*current].i, ijz->ijz[*current].j,
                        ijz->ijz[*current+1].i, ijz->ijz[*current+1].j,
                        pprev->i, pprev->j, pnext->i, pnext->j)
                *current = *current + 1;
            }
            LU_CHECK(luarray_pushxyz(log, xyz, pnext->i, pnext->j, pnext->z))
            pprev = pnext; nextisup = !nextisup;
        } else {
            *current = *current + 1;
            goto exit;
        }
    } while (*current < ijz->mem.used);
    LU_NO_CLEANUP
}

static int addstrip(lulog *log, luarray_ijz *ijz, size_t *current, ludata_ijz **index,
        ludata_ij bl, ludata_ij tr, luarray_xyz *xyz, luarray_int *offsets) {
    LU_STATUS
    ludata_ijz *p1 = &ijz->ijz[*current];
    ludata_ijz *p0 = index[ij2index(upleft(*p1), bl, tr)];
    ludata_ijz *p2 = index[ij2index(upright(*p1), bl, tr)];
    // if at least three points exist, add the first two and then add the rest
    if (p0 && p2) {
        LU_CHECK(luarray_pushint(log, offsets, xyz->mem.used))
        LU_CHECK(luarray_pushxyz(log, xyz, p0->i, p0->j, p0->z))
        LU_CHECK(luarray_pushxyz(log, xyz, p1->i, p1->j, p1->z))
        LU_CHECK(addpoints(log, ijz, current, p1, 1, index, bl, tr, xyz, offsets));
    } else {
        ludata_ijz *p3 = index[ij2index(right(*p1), bl, tr)];
        if (p3) {
            LU_ASSERT(*current + 1 < ijz->mem.used, LU_ERR, log,
                    "Unsorted points?  No data at %zu", *current + 1);
            LU_ASSERT(p3 == &ijz->ijz[*current + 1], LU_ERR, log,
                    "Unsorted points?  p3=(%d,%d), p1=(%d,%d), next=(%d,%d)",
                    p3->i, p3->j, p1->i, p1->j, ijz->ijz[*current + 1].i, ijz->ijz[*current + 1].j)
        }
        if (p2 && p3) {
            LU_CHECK(luarray_pushint(log, offsets, xyz->mem.used))
            LU_CHECK(luarray_pushxyz(log, xyz, p1->i, p1->j, p1->z))
            LU_CHECK(luarray_pushxyz(log, xyz, p2->i, p2->j, p2->z))
            LU_CHECK(addpoints(log, ijz, current, p2, 0, index, bl, tr, xyz, offsets));
        } else {
            *current = *current+1;
        }
    }
    LU_NO_CLEANUP
}

int lutriplex_strips(lulog *log, luarray_ijz *ijz, luarray_xyz **xyz, luarray_int **offsets) {
    LU_STATUS
    ludata_ij bl, tr;
    LU_CHECK(range(log, ijz, &bl, &tr, NULL))
    bl.i--; bl.j--; tr.i++; tr.j++;  // add border for failed lookups
    ludata_ijz **index = NULL;
    LU_CHECK(mkindex(log, ijz, bl, tr, &index))
    LU_CHECK(luarray_mkxyzn(log, xyz, 4 * ijz->mem.used))  // guess some overhead
    LU_CHECK(luarray_mkintn(log, offsets, tr.j - bl.j + 1))  // optimistic?
    size_t current = 0;
    while (current < ijz->mem.used) {
        LU_CHECK(addstrip(log, ijz, &current, index, bl, tr, *xyz, *offsets))
    }
    LU_CHECK(luarray_pushint(log, *offsets, (*xyz)->mem.used))
    luinfo(log, "Generated %zu triangle strips", (*offsets)->mem.used - 1);
LU_CLEANUP
    free(index);
    LU_RETURN
}
