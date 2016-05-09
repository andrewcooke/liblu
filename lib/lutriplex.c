
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


int lutriplex_free_perm(lulog *log, lutriplex_perm **perm, int prev_status) {
    if (*perm) free((*perm)->perm);
    free(perm);
    return prev_status;
}

int lutriplex_mkperm(lulog *log, lutriplex_perm **perm, int modulus, int length) {
    LU_STATUS
    LU_ALLOC(log, *perm, 1)
    LU_ALLOC(log, (*perm)->perm, length*2)
    (*perm)->modulus = modulus;
    (*perm)->length = length;

    LU_NO_CLEANUP
}

int lutriplex_default_perm(lulog *log, lutriplex_perm **perm) {
    return lutriplex_mkperm(log, perm, 8, 256);
}

double lutriplex_noise2(double pin, double qin) {
    return LU_OK;
}
