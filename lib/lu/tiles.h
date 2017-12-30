
#ifndef LU_TILES_H
#define LU_TILES_H

#include "log.h"
#include "random.h"
#include "data/ijz.h"
#include "structs.h"


// a re-implementation of perlin's simplex noise that:
// - uses coordinates on a triangular grid (NOT aligned with the triangular
//   grid implicit in the lusimplex 2D code)
// - supports patterns that tile (on whole triangles)
// - has configurable noise (permutation)
// - supports only 2D
// - has configurable noise vectors
//   see http://stackoverflow.com/a/21568753/181772
// - is not size_tended to be as efficient as lusimplex

// this code is based on the code in lusimplex (see credits there)

// the triangular mesh coordinates are not orthogonal.
// i arbitrarily pick the following two (of three) coordinates:
// - p is along the x axis
// - q is rotated 60 degrees anti-clock from p

struct lutle_xy;
struct lutle_tile;

typedef struct lutle_config {
    size_t n_grad;
    ludta_xy *grad;
    size_t n_perm;
    size_t *perm;
} lutle_config;

int lutle_freeconfig(lutle_config **config, int prev_status);
int lutle_mkconfig(lulog *log, lutle_config **config, luran *rand,
        size_t n_grad, double phase, size_t n_perm);
int lutle_defaultconfig(lulog *log, lutle_config **config, uint64_t seed);

int lutle_noise(lulog *log, lutle_config *conf,
        struct lutle_tile *tile, double pin, double qin, double *noise);

// tiles are (convex?) shapes that fit within the triangular (p,q) grid of the
// triplex and can be tiled without rotation.  corner 0 of the tile is the
// leftmost of the lowest corners. further corners (and edges) are numbered
// anti-clockwise.

// the "side" of a tile is the number of triplex triangle edges that fit
// on a (typical?) side.  the "subsamples" is the number of samples within
// a single triplex triangle when enumerating points.

// points are enumerated from corner 0 in raster fashion, left to right and
// ascending.  the integer coordinates are in subsample units.  edges are
// selected for output given the bitmask "edges".

// when octweight is 1, octave noise is weighted by 1/2 per octave (so
// fine details are progressively smaller).  a larger value emphasises the
// high frequency noise.

struct lutle_tile;

typedef int lutle_enumerate(struct lutle_tile *tile, lulog *log,
        lutle_config *config, uint edges, luary_ijz **ijz);
typedef int lutle_freetile(struct lutle_tile **tile, size_t prev_status);

// this function can modify its input to generate the appropriate gradients
// for tiling.
typedef void lutle_wrap(struct lutle_tile *tile, lulog *log, int *p, int *q);

typedef struct lutle_tile {
    size_t side;
    size_t subsamples;
    size_t octave;  // used internally while rendering
    double octweight;
    lutle_enumerate *enumerate;
    lutle_wrap *wrap;
    lutle_freetile *free;
    void *state;
} lutle_tile;

int lutle_mktriangle(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight);
int lutle_mkhexagon(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight);

int lutle_range(lulog *log, luary_ijz *ijz, ludta_ij *bl, ludta_ij *tr, double *zero);

// this is not geometrically correct.  instead, it uses a pattern like
//     x x x
//    x x x x
// and linearly interpolates the gaps horizontally
int lutle_rasterize(lulog *log, luary_ijz *ijz, size_t *nx, size_t *ny, double **data);

#endif
