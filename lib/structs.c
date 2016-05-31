
#include "lu/structs.h"

char *ludata_fxyzw2str(ludata_fxyzw xyzw, int n, char *buffer) {
    snprintf(buffer, n, "{%g,%g,%g,%g}", xyzw.x, xyzw.y, xyzw.z, xyzw.w);
    return buffer;
}