
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "lustatus.h"
#include "lumem.h"
#include "lustr.h"
#include "lulog.h"


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
    status = lustr_free(NULL, &state->line, status);
    free(state);
    free(*log);
    *log = NULL;
    LU_NO_CLEANUP2(prev_status)
}

int stream_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
    LU_STATUS
    if (level <= log->threshold) {
        stream_state *state = (stream_state*) log->state;
        LU_CHECK(lustr_nprintf(log, &state->line, log->max_line_length, "%s: ",
                 prefixes[level]));
        LU_CHECK(lustr_nappendfv(log, &state->line,
                 log->max_line_length - state->line.mem.used, format, ap));
        LU_CHECK(lustr_nappendf(log, &state->line,
                 log->max_line_length - state->line.mem.used, "\n"));
        fprintf(state->stream, "%s", state->line.c);
    }
    LU_NO_CLEANUP
}


int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold, int close) {
    LU_STATUS
    LU_ALLOC(*log, 1);
    LU_ALLOC_TYPE((*log)->state, 1, stream_state);
    stream_state *state = (stream_state*) (*log)->state;
    state->close = close;
    state->stream = stream;
    LU_CHECK(lustr_init(NULL, &state->line));
    (*log)->threshold = threshold;
    (*log)->max_line_length = LULOG_DEFAULT_MAX_LINE_LENGTH;
    (*log)->print = stream_printfv;
    (*log)->free = stream_free;
    LU_NO_CLEANUP
}

int lulog_mkstderr(lulog **log, lulog_level threshold) {
    return lulog_mkstream(log, stderr, threshold, 0);
}

int lulog_mkstdout(lulog **log, lulog_level threshold) {
    return lulog_mkstream(log, stdout, threshold, 0);
}


#define MKPRINT(level)\
int lu ## level(lulog *log, const char *format, ...) {\
    LU_STATUS\
    if (log) {\
        va_list ap;\
        va_start(ap, format);\
        LU_CHECK(stream_printfv(log, lulog_level_ ## level, format, ap));\
        LU_CLEANUP\
        va_end(ap);\
    }\
    LU_RETURN\
}

MKPRINT(error)
MKPRINT(warn)
MKPRINT(info)
MKPRINT(debug)
