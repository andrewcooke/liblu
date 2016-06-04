
#include <stdio.h>

#include "lu/structs.h"

char *ludta_fxyzw2str(ludta_fxyzw xyzw, int n, char *buffer) {
    snprintf(buffer, n, "{%g,%g,%g,%g}", xyzw.x, xyzw.y, xyzw.z, xyzw.w);
    return buffer;
}
