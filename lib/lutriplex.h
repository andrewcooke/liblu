
#ifndef LU_TRIPLEX_H
#define LU_TRIPLEX_H

#include "lulog.h"
#include "lurand.h"
#include "luarray.h"


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

struct lutriplex_xy;

typedef struct lutriplex_config {
    size_t n_grad;
    ludata_xy *grad;
    size_t n_perm;
    size_t *perm;
} lutriplex_config;

int lutriplex_free_config(lutriplex_config **config, int prev_status);
int lutriplex_mkconfig(lulog *log, lurand *rand, lutriplex_config **config,
        size_t n_grad, double phase, size_t n_perm);
int lutriplex_default_config(lulog *log, lutriplex_config **config);


// tiles are (convex?) shapes that fit within the triangular (p,q) grid of the
// triplex and can be tiled.  corner 0 is the leftmost of the lowest corners.
// further corners (and edges) are numbered anti-clock.

// the "side" of a tile is the number of triplex triangle edges that fit
// on a (typical?) side.  the "subsamples" is the number of samples within
// a single triplex triangle when enumerating points.

// points are enumerated from corner 0 in raster fashion, left to right and
// ascending.  the integer coordinates are in subsample units.  edges are
// selected for output given the bitmask "edges".

struct lutriplex_tile;

typedef int lutriplex_enumerate(struct lutriplex_tile *tile, lulog *log,
        lutriplex_config *config, ludata_ij corner0, uint edges,
        luarray_ijz **ijz);
typedef int lutriplex_freetile(struct lutriplex_tile **tile, size_t prev_status);

typedef struct lutriplex_tile {
    size_t side;
    size_t subsamples;
    lutriplex_enumerate *enumerate;
    lutriplex_freetile *free;
    void *state;
} lutriplex_tile;

int lutriplex_mktriangle(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples);

int lutriplex_mkhexagon(lulog *log, lutriplex_tile **tile, size_t side, size_t subsamples);

#endif
