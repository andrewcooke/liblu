
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lustatus.h"
#include "lumem.h"
#include "lulog.h"
#include "luminmax.h"
#include "lustr.h"


static LUMEM_MKFREE(internal_free, char)
static LUMEM_MKRESERVE(internal_reserve, char)


int lustr_free(lustr *str, int prev_status) {
    return internal_free(&str->c, &str->mem, prev_status);
}

int lustr_reserve(lulog *log, lustr *str, int n) {
    return internal_reserve(log, &str->c, &str->mem, n);
}

int lustr_mkn(lulog *log, lustr *str, size_t n) {
    LU_STATUS
    LU_ASSERT(n > 0, "Cannot alloc zero-length string", LU_ERR_ARG)
    *str = (lustr){NULL, .mem = LUMEM_ZERO};
    LU_CHECK(lustr_reserve(log, str, n));
    str->mem.used = 1;  // no need to set value as reserved is zeroed
    LU_NO_CLEANUP
}

int lustr_mk(lulog *log, lustr *str) {
    return lustr_mkn(log, str, 1);
}

int lustr_mkstr(lulog *log, lustr *str, const char *c) {
    LU_STATUS
    *str = (lustr){NULL, .mem = LUMEM_ZERO};
    int len = strlen(c);
    LU_CHECK(lustr_reserve(log, str, len + 1));
    memcpy(str->c, c, len);
    LU_NO_CLEANUP
}

int lustr_clear(lulog *log, lustr *str) {
    LU_STATUS
    if (!str->mem.capacity) LU_CHECK(lustr_mk(log, str));
    str->c[0] = '\0';
    str->mem.used = 1;
    LU_NO_CLEANUP
}


int lustr_print(lulog *log, lustr *str, const char *text) {
    return lustr_nprint(log, str, -1, text);
}

int lustr_nprint(lulog *log, lustr *str, int max_size, const char *text) {
    return lustr_nprintf(log, str, max_size, "%s", text);
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
    return lustr_nprintfv(log, str, -1, format, ap);
}

int lustr_nprintfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap) {
    LU_STATUS
    LU_CHECK(lustr_clear(log, str));
    LU_CHECK(lustr_nappendfv(log, str, max_size, format, ap));
    LU_NO_CLEANUP
}

int lustr_append(lulog *log, lustr *str, const char *text) {
    return lustr_nappend(log, str, -1, text);
}

int lustr_nappend(lulog *log, lustr *str, int max_size, const char *text) {
    return lustr_nappendf(log, str, max_size, "%s", text);
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
    return lustr_nappendfv(log, str, -1, format, ap);
}

int lustr_nappendfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap) {
    LU_STATUS
    va_list working;
    while (1) {
        va_copy(working, ap);
        int null_fix = str->mem.used ? 1 : 0;
        char *c = str->c + str->mem.used - null_fix;
        size_t space = str->mem.capacity - str->mem.used;
        size_t max_space = max_size < 0 ? space : min((size_t)max_size, space);
        int total = vsnprintf(c, max_space + null_fix, format, working);
        va_end(working);
//        ludebug(log, "Want to write %zu, have space for %zu, max is %d",
//                total, space, max_size);
        if (total < 0) {
            luerror(log, "Error formatting '%s': %s", format, strerror(errno));
            status = LU_ERR_IO;
            goto exit;
        } else if ((size_t)total <= max_space || (max_size >= 0 && space >= (size_t)max_size)) {
            str->mem.used += total;
            break;
        } else {
            size_t max_total = max_size < 0 ? total : min(max_size, total);
            LU_CHECK(lustr_reserve(log, str, max_total));
            ludebug(log, "Requested %zu; received %zu (used %zu)",
                    max_total, str->mem.capacity - str->mem.used, str->mem.used);
        }
    }
    LU_NO_CLEANUP
}

