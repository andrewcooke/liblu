
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lumem.h"
#include "lustatus.h"
#include "lustr.h"

LUMEM_MKFREE(interal_free, char)
LUMEM_MKRESERVE(internal_reserve, char)

int lustr_free(lulog *log, lustr *str, int prev_status) {
	return internal_free(log, &str->c, &str->mem, prev_status);
}

int lustr_reserve(lulog *log, lustr *str, int n) {
	return internal_reserve(log, &str->c, &str->mem, n);
}

int lustr_init(lulog *log, lustr *str) {
	LU_STATUS
	*str = (lustr){NULL, .mem = LUMEM_ZERO(char)};
	LU_CHECK(lustr_reserve(log, str, 1));
	str->mem.used = 1;  // no need to set value as reserved is zeroed
	LU_NO_CLEANUP
}

int lustr_initstr(lulog *log, lustr *str, const char *c) {
	LU_STATUS
	*str = (lustr){NULL, .mem = LUMEM_ZERO(char)};
	int len = strlen(c);
	LU_CHECK(lumem_reserve(log, (void**)&str->c, &str->mem, len + 1));
	memcpy(str->c, c, str->mem.unit);
	LU_NO_CLEANUP
}

int lustr_clear(lulog *log, lustr *str) {
	LU_STATUS
	if (!str->mem.available) LU_CHECK(lustr_mk(log, str));
	str->c[0] = '\0';
	str->mem.used = 1;
	LU_RETURN
}

int lustr_printf(lulog *log, lustr *str, const char *format, ...) {
	LU_STATUS
	va_list ap;
	va_start(ap, format);
	LU_CHECK(lustr_printfv(log, str, format, ap));
	LU_CLEANUP
	va_end(ap);
	LU_RETURN
}

int lustr_nprintf(lulog *log, lustr *str, int max_size, const char *format, ...) {
	LU_STATUS
	va_list ap;
	va_start(ap, format);
	LU_CHECK(lustr_nprintfv(log, str, max_size, format, ap));
	LU_CLEANUP
	va_end(ap);
	LU_RETURN
}

int lustr_printfv(lulog *log, lustr *str, const char *format, va_list ap) {
	LU_STATUS
	LU_CHECK(lustr_nprintfv(log, str, -1, format, ap));
	LU_NO_CLEANUP
}

int lustr_nprintfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap) {
	LU_STATUS
	LU_CHECK(lustr_clear(log, str));
	LU_CHECK(lustr_nappendfv(log, str, max_size, format, ap));
	LU_NO_CLEANUP
}

int lustr_appendf(lulog *log, lustr *str, const char *format, ...) {
	LU_STATUS
	va_list ap;
	va_start(ap, format);
	LU_CHECK(lustr_appendfv(log, str, format, ap));
	LU_CLEANUP
	va_end(ap);
	LU_RETURN
}

int lustr_nappendf(lulog *log, lustr *str, int max_size, const char *format, ...) {
	LU_STATUS
	va_list ap;
	va_start(ap, format);
	LU_CHECK(lustr_nappendfv(log, str, max_size, format, ap));
	LU_CLEANUP
	va_end(ap);
	LU_RETURN
}

int lustr_appendfv(lulog *log, lustr *str, const char *format, va_list ap) {
	LU_STATUS
	LU_CHECK(lustr_nappendfv(log, str, -1, format, ap));
	LU_NO_CLEANUP
}

int lustr_nappendfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap) {
	LU_STATUS
	char *c = str->c + str->mem.used - 1;  // subtract 1 to overwrite trailing NULL
	while (1) {
		size_t space = str->mem.available - str->mem.used + 1;
		size_t max_space = max_size < 0 ? space : min(max_size, space);
		int total = vsnprintf(c, max_space, format, ap);
		ludebug(log, "Want to write %zu, have space for %zu, max is %zu",
				total, space, max_size);
		if (total < 0) {
			luerror(log, "Error formatting '%s': %s", format, strerror(errno));
			status = LU_ERR_IO;
			goto exit;
		} else if (total > max_space && total <= max_size) {
			size_t max_total = max_size < 0 ? total : min(max_size, total);
			ludebug(log, "Reserving %zu", max_total);
			LU_CHECK(lustr_reserve(log, str, max_total));
		} else {
			str->mem.used += total;
			break;
		}
	}
	LU_NO_CLEANUP
}


