
#include <string.h>
#include <math.h>

#include "lustatus.h"
#include "luminmax.h"
#include "lustr.h"
#include "lulog.h"
#include "lumem.h"
#include "lugrey.h"


int lugrey_str(lulog *log, int *data, size_t nx, size_t ny, const char *scale, lustr *out) {
    LU_STATUS
    size_t i, j, warn = 0;
    int n = strlen(scale) - 1;
    for (j = 0; j < ny; ++j) {
        for (i = 0; i < nx; ++i) {
            if (*data < 0 || *data > n) {
                if (!warn) {
                    luwarn(log, "Greyscale data outside range (0, %d): %d", n, *data);
                    warn = 1;
                }
            }
            LU_CHECK(lustr_appendf(log, out, "%c", scale[min(n, max(0, *data))]))
            data++;
        }
        LU_CHECK(lustr_append(log, out, "\n"))
    }
    LU_NO_CLEANUP
}

int lugrey_quantize(lulog *log, double *data, int n, int levels, int **out) {
    LU_STATUS;
    int i;
    double lo = *data, hi = *data, delta;
    for (i = 0; i < n; ++i) {
        lo = min(lo, data[i]);
        hi = max(hi, data[i]);
    }
    luinfo(log, "Scaling %5.4g - %5.4g to 0 - %d", lo, hi, levels-1);
    delta = (hi - lo) / levels;
    LU_ALLOC(log, *out, n)
    for (i = 0; i < n; ++i) {
        (*out)[i] = min(levels-1, max(0, floor((data[i] - lo) / delta)));
    }
    LU_NO_CLEANUP
}
