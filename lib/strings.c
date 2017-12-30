
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <lu/internal.h>

#include "lu/dynamic_memory.h"
#include "lu/log.h"
#include "lu/minmax.h"
#include "lu/strings.h"


static LUMEM_MKFREE(internal_free, char)
static LUMEM_MKRESERVE(internal_reserve, char)


int lustr_free(lustr *str, int prev_status) {
    return internal_free(&str->c, &str->mem, prev_status);
}

int lustr_reserve(lulog *log, lustr *str, int n) {
    return internal_reserve(log, &str->c, &str->mem, n);
}

int lustr_mkn(lulog *log, lustr *str, size_t n) {
    int status = LU_OK;
    assert(n > 0, LU_ERR_ARG, log, "Cannot alloc zero-length string")
    *str = (lustr){NULL, .mem = LUMEM_ZERO};
    try(lustr_reserve(log, str, n));
    str->mem.used = 1;  // no need to set value as reserved is zeroed
    finally:return status;
}

int lustr_mk(lulog *log, lustr *str) {
    return lustr_mkn(log, str, 1);
}

int lustr_mkstr(lulog *log, lustr *str, const char *c) {
    int status = LU_OK;
    *str = (lustr){NULL, .mem = LUMEM_ZERO};
    int len = strlen(c);
    try(lustr_reserve(log, str, len + 1));
    memcpy(str->c, c, len);
    finally:return status;
}

int lustr_clear(lulog *log, lustr *str) {
    int status = LU_OK;
    if (!str->mem.capacity) try(lustr_mk(log, str));
    str->c[0] = '\0';
    str->mem.used = 1;
    finally:return status;
}


int lustr_add(lulog *log, lustr *str, char c) {
    int status = LU_OK;
    try(lustr_reserve(log, str, 1))
    str->c[str->mem.used-1] = c;
    str->c[str->mem.used++] = '\0';
    finally:return status;
}

int lustr_print(lulog *log, lustr *str, const char *text) {
    return lustr_nprint(log, str, -1, NULL, text);
}

int lustr_nprint(lulog *log, lustr *str, int max_size, int *all_chars, const char *text) {
    return lustr_nprintf(log, str, max_size, all_chars, "%s", text);
}

int lustr_printf(lulog *log, lustr *str, const char *format, ...) {
    int status = LU_OK;
    va_list ap;
    va_start(ap, format);
    try(lustr_vprintf(log, str, format, ap));
    finally:
    va_end(ap);
    return status;
}

int lustr_nprintf(lulog *log, lustr *str, int max_size, int *all_chars, const char *format, ...) {
    int status = LU_OK;
    va_list ap;
    va_start(ap, format);
    try(lustr_vnprintf(log, str, max_size, all_chars, format, ap));
    finally:
    va_end(ap);
    return status;
}

int lustr_vprintf(lulog *log, lustr *str, const char *format, va_list ap) {
    return lustr_vnprintf(log, str, -1, NULL, format, ap);
}

int lustr_vnprintf(lulog *log, lustr *str, int max_size, int *all_chars, const char *format, va_list ap) {
    int status = LU_OK;
    try(lustr_clear(log, str));
    try(lustr_vnappendf(log, str, max_size, all_chars, format, ap));
    finally:return status;
}

int lustr_append(lulog *log, lustr *str, const char *text) {
    return lustr_nappend(log, str, -1, NULL, text);
}

int lustr_nappend(lulog *log, lustr *str, int max_size, int *all_chars, const char *text) {
    return lustr_nappendf(log, str, max_size, all_chars, "%s", text);
}

int lustr_appendf(lulog *log, lustr *str, const char *format, ...) {
    int status = LU_OK;
    va_list ap;
    va_start(ap, format);
    try(lustr_vappendf(log, str, format, ap));
    finally:
    va_end(ap);
    return status;
}

int lustr_nappendf(lulog *log, lustr *str, int max_size, int *all_chars, const char *format, ...) {
    int status = LU_OK;
    va_list ap;
    va_start(ap, format);
    try(lustr_vnappendf(log, str, max_size, all_chars, format, ap));
    finally:
    va_end(ap);
    return status;
}

int lustr_vappendf(lulog *log, lustr *str, const char *format, va_list ap) {
    return lustr_vnappendf(log, str, -1, NULL, format, ap);
}

int lustr_vnappendf(lulog *log, lustr *str, int max_chars, int *all_chars, const char *format, va_list ap) {
    int status = LU_OK;
    va_list working;
    int null_present = str->mem.used > 0;
    while (1) {
        // this must be inside the loop because str->c is reallocated
        // also, the offset is never negative because null_present is 0 if mem.used is 0
        char *c = str->c + str->mem.used - null_present;
        va_copy(working, ap);
        int space_for_chars = str->mem.capacity - str->mem.used - !null_present;
        int max_space_for_chars = max_chars < 0 ? space_for_chars : min((size_t)max_chars, space_for_chars);
        int total_chars = vsnprintf(c, max_space_for_chars + 1, format, working);
        va_end(working);
        if (total_chars < 0) {
            luerror(log, "Error formatting '%s': %s", format, strerror(errno));
            status = LU_ERR_IO;
            goto finally;
        }
        int truncated = total_chars > max_space_for_chars;
        int memory_limited = max_chars > -1 && max_chars <= space_for_chars;
        if (!truncated || memory_limited) {
            if (all_chars) *all_chars = total_chars;
            str->mem.used += total_chars + !null_present;
            break;
        }
        try(lustr_reserve(log, str, total_chars + !null_present));
    }
    finally:return status;
}
