
#include <math.h>

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


int lutriplex_free_config2(lulog *log, lutriplex_config2 **config, int prev_status) {
    if (*config) {
        free((*config)->grad);
        free((*config)->perm);
    }
    free(config);
    return prev_status;
}

int lutriplex_mkconfig2(lulog *log, lutriplex_config2 **config,
        int n_grad, double phase, int n_perm) {
    int i;
    LU_STATUS
    LU_ALLOC(log, *config, 1)
    LU_ALLOC(log, (*config)->grad, n_grad)
    LU_ALLOC(log, (*config)->perm, n_perm * 2)
    (*config)->n_grad = n_grad;
    (*config)->n_perm = n_perm;
    for (i = 0; i < n_grad; ++i) {
        double theta = phase + 2*M_PI / n_grad;
        lutriplex_grad2 grad = {cos(theta), sin(theta)};
        (*config)->grad[i] = grad;
    }
    LU_NO_CLEANUP
}

int lutriplex_default_config2(lulog *log, lutriplex_config2 **config) {
    return lutriplex_mkconfig2(log, config, 0, 9, 256);
}

double lutriplex_noise2(double pin, double qin) {
    return LU_OK;
}
