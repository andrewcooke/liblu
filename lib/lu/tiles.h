
#ifndef LU_TILES_H
#define LU_TILES_H

#include "log.h"
#include "random.h"
#include "arrays.h"


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

struct lutile_xy;
struct lutile_tile;

typedef struct lutile_config {
    size_t n_grad;
    ludata_xy *grad;
    size_t n_perm;
    size_t *perm;
} lutile_config;

int lutile_freeconfig(lutile_config **config, int prev_status);
int lutile_mkconfig(lulog *log, lutile_config **config, lurand *rand,
        size_t n_grad, double phase, size_t n_perm);
int lutile_defaultconfig(lulog *log, lutile_config **config, uint64_t seed);

int lutile_noise(lulog *log, lutile_config *conf,
        struct lutile_tile *tile, double pin, double qin, double *noise);

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

struct lutile_tile;

typedef int lutile_enumerate(struct lutile_tile *tile, lulog *log,
        lutile_config *config, uint edges, luarray_ijz **ijz);
typedef int lutile_freetile(struct lutile_tile **tile, size_t prev_status);

// this function can modify its input to generate the appropriate gradients
// for tiling.
typedef void lutile_wrap(struct lutile_tile *tile, lulog *log, int *p, int *q);

typedef struct lutile_tile {
    size_t side;
    size_t subsamples;
    size_t octave;  // used internally while rendering
    double octweight;
    lutile_enumerate *enumerate;
    lutile_wrap *wrap;
    lutile_freetile *free;
    void *state;
} lutile_tile;

int lutile_mktriangle(lulog *log, lutile_tile **tile,
        size_t side, size_t subsamples, double octweight);
int lutile_mkhexagon(lulog *log, lutile_tile **tile,
        size_t side, size_t subsamples, double octweight);

// this is not geometrically correct.  instead, it uses a pattern like
//     x x x
//    x x x x
// and linearly interpolates the gaps horizontally
int lutile_rasterize(lulog *log, luarray_ijz *ijz, size_t *nx, size_t *ny, double **data);

// generate triangle strips (each strip offset into a single array)
int lutile_strips(lulog *log, luarray_ijz *ijz, luarray_uint32 **indices, luarray_uint32 **offsets);
int lutile_ijz2fxyzw(lulog *log, luarray_ijz *ijz, float step, luarray_fxyzw **fxyzw);


#endif
