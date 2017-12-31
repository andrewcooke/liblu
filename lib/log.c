
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include "lu/internal.h"
#include "lu/memory.h"
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
	int status = LU_OK;
	stream_state *state = (stream_state*) (*log)->state;
	fflush(state->stream);
	if (state->close) fclose(state->stream);
	status = lustr_free(&state->line, status);
	free(state);
	free(*log); *log = NULL;  // TODO - macro
	return status;
}

int stream_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
	int status = LU_OK;
	if (level <= log->threshold) {
		stream_state *state = (stream_state*) log->state;
		try(lustr_printf(NULL, &state->line, "%s: ", prefixes[level]));
		try(lustr_vappendf(NULL, &state->line, format, ap));
		try(lustr_append(NULL, &state->line, "\n"));
		fprintf(state->stream, "%s", state->line.c);
	}
	finally:
	return status;
}

int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold, int close) {
	int status = LU_OK;
	LU_ALLOC(NULL, *log, 1);
	LU_ALLOC_TYPE(NULL, (*log)->state, 1, stream_state);
	stream_state *state = (stream_state*) (*log)->state;
	state->close = close;
	state->stream = stream;
	try(lustr_mk(NULL, &state->line));
	(*log)->threshold = threshold;
	(*log)->printfv = stream_printfv;
	(*log)->free = stream_free;
	finally:return status;
}

int lulog_mkstderr(lulog **log, lulog_level threshold) {
	return lulog_mkstream(log, stderr, threshold, 0);
}

int lulog_mkstdout(lulog **log, lulog_level threshold) {
	return lulog_mkstream(log, stdout, threshold, 0);
}


static const int priorities[] = {LOG_ERR, LOG_WARNING, LOG_INFO, LOG_DEBUG};

int syslog_free(lulog **log, int prev_status) {
	int status = LU_OK;
	free(*log); *log = NULL;
	return status;
}

int syslog_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
	int status = LU_OK;
	if (level <= log->threshold) vsyslog(priorities[level], format, ap);
	finally:
	return status;
}

int lulog_mksyslog(lulog **log, const char *ident, lulog_level threshold) {
	int status = LU_OK;
	LU_ALLOC(NULL, *log, 1);
	(*log)->threshold = threshold;
	(*log)->printfv = syslog_printfv;
	(*log)->free = syslog_free;
	openlog(ident, 0, LOG_USER);
	finally:
	return status;
}


// this frees the string that was returned in the constructor, so should be
// called after that data is used.
int string_free(lulog **log, int prev_status) {
	int status = LU_OK;
	lustr *string = (lustr*)(*log)->state;
	status = lustr_free(string, status);
	free(string);
	free(*log); *log = NULL;
	return status;
}

int string_printfv(lulog *log, lulog_level level, const char *format, va_list ap) {
	int status = LU_OK;
	if (level <= log->threshold) {
		lustr *string = (lustr*)log->state;
		size_t zero = string->mem.used;
		try(lustr_appendf(NULL, string, "%s: ", prefixes[level]))
        		try(lustr_vappendf(NULL, string, format, ap))
				try(lustr_append(NULL, string, "\n"))
	}
	finally:
	return status;
}

int lulog_mkstring(lulog **log, lustr **string, lulog_level threshold) {
	int status = LU_OK;
	LU_ALLOC_TYPE(NULL, *string, 1, lustr)
	try(lustr_mk(NULL, *string))
	LU_ALLOC(NULL, *log, 1)
	(*log)->state = *string;
	(*log)->threshold = threshold;
	(*log)->printfv = string_printfv;
	(*log)->free = string_free;
	finally:
	return status;
}


#define MKPRINT(level)\
		int lu ## level(lulog *log, const char *format, ...) {\
	int status = LU_OK;\
	va_list ap;\
	va_start(ap, format);\
	if (log) {\
		try(log->printfv(log, lulog_level_ ## level, format, ap));\
	} else {\
		fprintf(stderr, "%s: ", prefixes[lulog_level_ ## level]);\
		vfprintf(stderr, format, ap);\
	}\
	finally:\
	va_end(ap);\
	return status;\
}

MKPRINT(error)
MKPRINT(warn)
MKPRINT(info)
MKPRINT(debug)

int lulog_lines(lulog *log, lulog_level level, const char *lines) {
	int status = LU_OK;
	lustr line = {};
	try(lustr_mk(log, &line))
    		const char *src = lines;
	while (*src) {
		try(lustr_clear(log, &line))
        		while (*src && *src != '\n') lustr_add(log, &line, *src++);
		switch(level) {
		case lulog_level_debug: ludebug(log, "%s", line.c); break;
		case lulog_level_info: luinfo(log, "%s", line.c); break;
		case lulog_level_warn: luwarn(log, "%s", line.c); break;
		default: luerror(log, "%s", line.c); break;
		}
		if (*src == '\n') src++;
	}
	finally:
	status = lustr_free(&line, status);
	return status;
}

int lulog_printf(lulog *log, lulog_level level, const char *format, ...) {
	int status = LU_OK;
	if (log) {
		va_list ap;
		va_start(ap, format);
		try(log->printfv(log, level, format, ap));
		finally:
		va_end(ap);
	}
	return status;
}
