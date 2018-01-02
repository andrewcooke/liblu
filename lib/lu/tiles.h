
#ifndef LU_TILES_H
#define LU_TILES_H

#include "log.h"
#include "random.h"
#include "data/ijz.h"
#include "data/xy.h"
#include "data/ij.h"

/**
 * @file
 *
 * @brief A tileable 2D noise similar to Perlin's simplex approach.
 *
 * The code here:
 * - uses coordinates on a triangular grid (NOT aligned with the triangular
 *   grid implicit in the lusimplex 2D code)
 * - supports patterns that tile (on whole triangles)
 * - has configurable noise (permutation)
 * - supports only 2D
 * - has configurable noise vectors
 *   see http://stackoverflow.com/a/21568753/181772
 * - is not size_tended to be as efficient as lusimplex
 *
 * It is based on the code in `simplex.h` (see credits there)
 *
 * The triangular mesh coordinates are not orthogonal.
 * I arbitrarily pick the following two (of three) coordinates:
 * - p is along the x axis
 * - q is rotated 60 degrees anti-clock from p
 *
 * The main routine is `lutle_noise()`.  To call that you typically
 * construct a `lutle_config` instance that fixes the noise parameters
 * and a `lutle_tile` instance that describes the tiling.
 */
/// @{


/// The structure used to configure a tile.
typedef struct {
    size_t n_grad;  ///< The number of evenly spaced points on a circle used for gradients.
    ludta_xy *grad;  ///< Evenly spaced points on a circle.
    size_t n_perm;  ///< The number of permutations used as a lookup to select gradients.
    size_t *perm;  ///< Permutations used as a lookup to select gradients.
} lutle_config;

/// Free a configuration.
int lutle_config_free(lutle_config **config, int prev_status);
/// Generate a configuration from the given parameters and random number source.
int lutle_config_mk(lulog *log, lutle_config **config, luran *rand,
        size_t n_grad, double phase, size_t n_perm);
/// Generate a configuration with fixed parameters and random number algorithm, using the given random seed.
int lutle_config_default(lulog *log, lutle_config **config, uint64_t seed);


/**
 * @struct lutle_tile
 *
 * @brief A tile describes a tileable piece of noise.
 *
 * Tiles are (convex?) shapes that fit within the triangular (p,q) grid of the
 * noise and can be tiled without rotation.  Corner 0 of the tile is the
 * leftmost of the lowest corners. further corners (and edges) are numbered
 * anti-clockwise.
 *
 * Most of the details of a tile depend on the `enumerate` and `wrap`
 * functions - the struct is an interface to particular implementations.
 *
 * The "side" of a tile is the number of triangle edges that fit on a
 * (typical?) side.  The "subsamples" is the number of samples within
 * a single triangle when enumerating points (this oversamples the noise).
 *
 * Points are enumerated from corner 0 in raster fashion, left to right and
 * ascending.  The integer coordinates are in subsample units.  Edges are
 * selected for output given the bitmask "edges" (following a convention
 * specific to the tile type and presumably undertood by the caller).
 *
 * When octweight is 1, octave noise is weighted by 1/2 per octave (so
 * fine details are progressively smaller).  A larger value emphasises the
 * high frequency noise.
 */
struct lutle_tile;

/// Given a tile and noise configuration generate the (i,j,z) noise for all points.
typedef int lutle_enumerate(struct lutle_tile *tile, lulog *log,
        lutle_config *config, uint edges, luary_ijz **ijz);
/// Modify `p` and `q` so that they refer to a canonical tile instance.
typedef void lutle_wrap(struct lutle_tile *tile, lulog *log, int *p, int *q);
/// Free the tile.
typedef int lutle_tile_free(struct lutle_tile **tile, size_t prev_status);

typedef struct lutle_tile {
    size_t side;  ///< The number of triangle edges on a (typical?) side.
    size_t subsamples;  ///< The degree of oversampling of the noise.
    size_t octave;  ///< Used internally while rendering
    double octweight;  ///< Weight for octave noise.
    lutle_enumerate *enumerate;  ///< Function to generate noise at all points.
    lutle_wrap *wrap;  ///< Function to wrap coordinates to a canonincal tile.
    lutle_tile_free *free;  ///< Function to free tile.
    void *state;  ///< Opaque state used by the tile.
} lutle_tile;

/// Generate a triangular tile.
int lutle_triangle_mk(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight);
/// Generate a hexagonal tile.
int lutle_hexagon_mk(lulog *log, lutle_tile **tile,
        size_t side, size_t subsamples, double octweight);
/// Find extent of coordinates in (i,j,z) array.
int lutle_range(lulog *log, luary_ijz *ijz, ludta_ij *bl, ludta_ij *tr, double *zero);

/**
 * @brief Interpolate (i,j,z) tuples on a rectangular array.
 *
 * This is not geometrically correct.  instead, it uses a pattern like
<PRE>
       x x x
      x x x x
</PRE>
 * and linearly interpolates the gaps horizontally
 */
int lutle_rasterize(lulog *log, luary_ijz *ijz, size_t *nx, size_t *ny, double **data);

/// Calculate the height (noise).
int lutle_noise(lulog *log, lutle_config *conf,
        lutle_tile *tile, double pin, double qin, double *noise);

/// @}
#endif
