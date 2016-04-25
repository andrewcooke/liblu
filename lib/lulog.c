
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "lumemory.h"
#include "lustatus.h"
#include "lulog.h"

typedef struct stream_state {
	FILE *stream;
	lulog_level threshold;
} stream_state;

#define MKPRINT(level)\
int stream_ ## level(lulog *log, const char *format, ...) { \
	LU_STATUS                                               \
	va_list ap;                                             \
	stream_state *state = (stream_state*)log->state;        \
	if (state->threshold <= lulog_level_ ## level) {        \
		va_start(ap, format);                               \
		fprintf(state->stream, "%s: ", #level);             \
		vfprintf(state->stream, format, ap);                \
		fprintf(state->stream, "\n");                       \
		va_end(ap);                                         \
	}                                                       \
	LU_RETURN                                               \
}

MKPRINT(debug)
MKPRINT(info)
MKPRINT(warn)
MKPRINT(error)
MKPRINT(fatal)

int stream_free(lulog **log, int prev_status) {
	stream_state *state = (stream_state*)(*log)->state;
	free(state);
	free(*log);
	*log = NULL;
	return prev_status;
}

int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold) {
	LU_STATUS
	LU_ALLOC(*log, 1);
	LU_ALLOC_TYPE((*log)->state, 1, stream_state);
	stream_state *state = (stream_state*)(*log)->state;
	state->threshold = threshold;
	state->stream = stream;
	(*log)->debug = stream_debug;
	(*log)->info = stream_info;
	(*log)->warn = stream_warn;
	(*log)->error = stream_error;
	(*log)->fatal = stream_fatal;
	(*log)->free = stream_free;
	LU_RETURN
}

int lulog_mkstderr(lulog **log, lulog_level threshold) {
	return lulog_mkstream(log, stderr, threshold);
}

int lulog_mkstdout(lulog **log, lulog_level threshold) {
	return lulog_mkstream(log, stdout, threshold);
}

