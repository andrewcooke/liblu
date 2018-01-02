
#include <lu/arrays.h>
#include "lu/data/fxyzw.h"

LUARY_MKBASE(fxyzw, ludta_fxyzw, fxyzw)

int luary_fxyzw_push(lulog *log, luary_fxyzw *fxyzw, float x, float y, float z, float w) {
    int status = LU_OK;
    try(luary_fxyzw_res(log, fxyzw, 1));
    fxyzw->fxyzw[fxyzw->mem.used++] = (ludta_fxyzw){x, y, z, w};
    finally:
	return status;
}

LUARY_MKDUMP(luary_fxyzw_dump, luary_fxyzw, "{%g,%g,%g,%g}",
		ptr->fxyzw[i].x, ptr->fxyzw[i].y, ptr->fxyzw[i].z, ptr->fxyzw[i].w)

char *ludta_fxyzw_str(ludta_fxyzw xyzw, int n, char *buffer) {
    snprintf(buffer, n, "{%g,%g,%g,%g}", xyzw.x, xyzw.y, xyzw.z, xyzw.w);
    return buffer;
}
