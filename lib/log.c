
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include "lu/status.h"
#include "lu/dynamic_memory.h"
#include "lu/strings.h"
#include "lu/log.h"


static const char* const prefixes[] = {
    "error",
    "warn",
    "info",
    "debug"
};


typedef struct stream_state {
    FILE *stream;
    int close;
    lustr line;
} stream_state;

int stream_free(lulog **log, int prev_status) {
    LU_STATUS
    stream_state *state = (stream_state*) (*log)->state;
    fflush(state->stream);
    if (state->close) fclose(state->stream);
    status = lustr_free(&state->line, status);
    free(state);
    free(*log); *log = NULL;
    LU_NO_CLEANUP2(prev_status)
}

int stream_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
    LU_STATUS
    if (level <= log->threshold) {
        stream_state *state = (stream_state*) log->state;
        LU_CHECK(lustr_sprintf(NULL, &state->line, "%s: ", prefixes[level]));
        LU_CHECK(lustr_vappendf(NULL, &state->line, format, ap));
        LU_CHECK(lustr_append(NULL, &state->line, "\n"));
        fprintf(state->stream, "%s", state->line.c);
    }
    LU_NO_CLEANUP
}

int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold, int close) {
    LU_STATUS
    LU_ALLOC(NULL, *log, 1);
    LU_ALLOC_TYPE(NULL, (*log)->state, 1, stream_state);
    stream_state *state = (stream_state*) (*log)->state;
    state->close = close;
    state->stream = stream;
    LU_CHECK(lustr_mk(NULL, &state->line));
    (*log)->threshold = threshold;
    (*log)->printfv = stream_printfv;
    (*log)->free = stream_free;
    LU_NO_CLEANUP
}

int lulog_mkstderr(lulog **log, lulog_level threshold) {
    return lulog_mkstream(log, stderr, threshold, 0);
}

int lulog_mkstdout(lulog **log, lulog_level threshold) {
    return lulog_mkstream(log, stdout, threshold, 0);
}


static const int priorities[] = {LOG_ERR, LOG_WARNING, LOG_INFO, LOG_DEBUG};

int syslog_free(lulog **log, int prev_status) {
    LU_STATUS
    free(*log); *log = NULL;
    LU_NO_CLEANUP2(prev_status)
}

int syslog_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
    LU_STATUS
    if (level <= log->threshold) vsyslog(priorities[level], format, ap);
    LU_NO_CLEANUP
}

int lulog_mksyslog(lulog **log, const char *ident, lulog_level threshold) {
    LU_STATUS
    LU_ALLOC(NULL, *log, 1);
    (*log)->threshold = threshold;
    (*log)->printfv = syslog_printfv;
    (*log)->free = syslog_free;
    openlog(ident, 0, LOG_USER);
    LU_NO_CLEANUP
}


// this frees the string that was returned in the constructor, so should be
// called after that data is used.
int string_free(lulog **log, int prev_status) {
    LU_STATUS
    lustr *string = (lustr*)(*log)->state;
    status = lustr_free(string, status);
    free(string);
    free(*log); *log = NULL;
    LU_NO_CLEANUP2(prev_status)
}

int string_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
    LU_STATUS
    if (level <= log->threshold) {
        lustr *string = (lustr*)log->state;
        size_t zero = string->mem.used;
        LU_CHECK(lustr_appendf(NULL, string, "%s: ", prefixes[level]))
        LU_CHECK(lustr_vappendf(NULL, string, format, ap))
        LU_CHECK(lustr_append(NULL, string, "\n"))
    }
    LU_NO_CLEANUP
}

int lulog_mkstring(lulog **log, lustr **string, lulog_level threshold) {
    LU_STATUS
    LU_ALLOC_TYPE(NULL, *string, 1, lustr)
    LU_CHECK(lustr_mk(NULL, *string))
    LU_ALLOC(NULL, *log, 1)
    (*log)->state = *string;
    (*log)->threshold = threshold;
    (*log)->printfv = string_printfv;
    (*log)->free = string_free;
    LU_NO_CLEANUP
}


#define MKPRINT(level)\
int lu ## level(lulog *log, const char *format, ...) {\
    LU_STATUS\
    va_list ap;\
    va_start(ap, format);\
    if (log) {\
        LU_CHECK(log->printfv(log, lulog_level_ ## level, format, ap));\
    } else {\
        fprintf(stderr, "%s: ", prefixes[lulog_level_ ## level]);\
        vfprintf(stderr, format, ap);\
    }\
    LU_CLEANUP\
    va_end(ap);\
    LU_RETURN\
}

MKPRINT(error)
MKPRINT(warn)
MKPRINT(info)
MKPRINT(debug)

int lulog_lines(lulog *log, lulog_level level, const char *lines) {
    LU_STATUS
    lustr line = {};
    LU_CHECK(lustr_mk(log, &line))
    const char *src = lines;
    while (*src) {
        LU_CHECK(lustr_clear(log, &line))
        while (*src && *src != '\n') lustr_add(log, &line, *src++);
        switch(level) {
        case lulog_level_debug: ludebug(log, "%s", line.c); break;
        case lulog_level_info: luinfo(log, "%s", line.c); break;
        case lulog_level_warn: luwarn(log, "%s", line.c); break;
        default: luerror(log, "%s", line.c); break;
        }
        if (*src == '\n') src++;
    }
LU_CLEANUP
    status = lustr_free(&line, status);
    LU_RETURN
}

int lulog_printf(lulog *log, lulog_level level, const char *format, ...) {
    LU_STATUS
    if (log) {
        va_list ap;
        va_start(ap, format);
        LU_CHECK(log->printfv(log, level, format, ap));
        LU_CLEANUP
        va_end(ap);
    }
    LU_RETURN
}
