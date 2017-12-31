
#include <string.h>
#include <math.h>

#include "lu/internal.h"
#include "lu/minmax.h"
#include "lu/strings.h"
#include "lu/log.h"
#include "lu/memory.h"
#include "lu/greyscale.h"


int lugry_str(lulog *log, int *data, size_t nx, size_t ny, const char *scale, lustr *out) {
    int status = LU_OK;
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
            try(lustr_appendf(log, out, "%c", scale[min(n, max(0, *data))]))
            data++;
        }
        try(lustr_append(log, out, "\n"))
    }
    finally:return status;
}

int lugry_quantize(lulog *log, double *data, int n, int levels, int **out) {
    int status = LU_OK;;
    int i;
    double lo = *data, hi = *data, delta;
    for (i = 0; i < n; ++i) {
        lo = min(lo, data[i]);
        hi = max(hi, data[i]);
    }
    assert(lo != hi, LU_ERR, log, "Constant data")
    luinfo(log, "Scaling %5.4g - %5.4g to 0 - %d", lo, hi, levels-1);
    delta = (hi - lo) / levels;
    LU_ALLOC(log, *out, n)
    for (i = 0; i < n; ++i) {
        (*out)[i] = min(levels-1, max(0, floor((data[i] - lo) / delta)));
    }
    finally:return status;
}
