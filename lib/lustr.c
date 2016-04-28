
#include <stdlib.h>
#include <string.h>

#include "lumem.h"
#include "lustatus.h"
#include "lustr.h"


int lustr_mk(lulog *log, lustr *str) {
	LU_STATUS
	*str = (lustr){NULL, .mem = LU_MKMEM(char)};
	LU_ASSERT(str->mem.unit == 1, "Non-standard char size", LU_ERR)
	LU_CHECK(lumem_reserve(log, (void**)&str->c, &str->mem, 1));
	str->mem.used = 1;  // no need to set value
	LU_NO_CLEANUP
}

int lustr_mkstr(lulog *log, lustr *str, const char *c) {
	LU_STATUS
	*str = (lustr){NULL, .mem = LU_MKMEM(char)};
	int len = strlen(c);
	LU_CHECK(lumem_reserve(log, (void**)&str->c, &str->mem, len + 1));
	lustr_append(log, str, "%s", c);
	LU_NO_CLEANUP
}

int lustr_free(lulog *log, lustr *str, int prev_status) {
	LU_STATUS
	free(str->c);
	str->mem.available = 0;
	str->mem.used = 0;
	LU_NO_CLEANUP2(prev_status)
}

int lustr_printf(lulog *log, lustr *str, const char *format, ...) {
	LU_STATUS
	LU_CHECK(lustr_truncate(log, str, 0));
	LU_CHECK(lustr_appendfv())
}

int lustr_nprintf(lulog *log, lustr *str, int max_size, const char *format, ...);


int lustr_append(lulog *log, lustr *str, const char *format, ...) {
	LU_STATUS
	va_list ap;
	va_start(ap, format);
	char *c = str->c + str->mem.used;
	while (1) {
		size_t n = str->mem.available - str->mem.used;
		int m = vsnprintf(c, n, format, ap);
		if (m < 0) {
			luerror(log, "Error formatting '%s': %s", format, strerror(errno));
			status = LU_ERR_IO;
			goto exit;
		} else if (m > n) {
			LU_CHECK(lumem_reserve(&str->c, &str->mem, m - n));
		} else {
			str->mem.used += m;
			break;
		}
	}
	LU_CLEANUP
	va_end(ap);
	LU_RETURN
}

int lustr_nappend(struct lustr *str, int max_size, const char *format, ...) {
	LU_STATUS
	LU_RETURN
}

int lustr_truncate(struct lustr *str, int max_size) {
	LU_STATUS
	LU_RETURN
}
