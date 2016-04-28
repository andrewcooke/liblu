
#include <stdlib.h>
#include <strings.h>

#include "lu.h"
#include "lustatus.h"
#include "lumem.h"


int lumem_reset(lulog *log, void **ptr, lumem *mem) {
	LU_STATUS;

	LU_NO_CLEANUP;
}

int lumem_reserve(lulog *log, void **ptr, lumem *mem, size_t n) {
	LU_STATUS
	if (mem->available >= n) goto exit;
	int required = mem->available > 0 ? mem->available : 1;
	while (required - mem->used < n) required *= 2;
	if (!(*ptr = realloc(*ptr, mem->unit * required))) {
		luerror(log, "Cannot allocate %zu bytes", mem->unit * required);
		status = LU_ERR_MEM;
		goto exit;
	}
	mem->available = required;
	memset(*ptr + mem->unit * mem->used, 0, mem->unit * required);
	LU_NO_CLEANUP
}
