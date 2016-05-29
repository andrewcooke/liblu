
#include "lu/structs.h"

#define BUFLEN 100

char *ludata_fxyzw2str(ludata_fxyzw xyzw) {
    char *buffer[BUFLEN];
    snprintf(buffer, BUFLEN, "{%g,%g,%g,%g}", xyzw.x, xyzw.y, xyzw.z, xyzw.w);
    return buffer;
}
