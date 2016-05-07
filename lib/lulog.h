
#ifndef LU_LOG_H
#define LU_LOG_H

#include <stdio.h>
#include <stdarg.h>

struct lulog;

#include "lustr.h"


typedef enum lulog_level {
	lulog_level_error,
	lulog_level_warn,
	lulog_level_info,
	lulog_level_debug
} lulog_level;

#define LULOG_DEFAULT_MAX_LINE_LENGTH 1000

typedef int lulog_printfv(struct lulog *log, lulog_level level, const char *format, va_list ap);
typedef int lulog_free(struct lulog **log, int status);

typedef struct lulog {
	lulog_level threshold;
	size_t max_line_length;
	lulog_printfv *printfv;
	lulog_free *free;
	void *state;
} lulog;

int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold, int close);
int lulog_mkstderr(lulog **log, lulog_level threshold);
int lulog_mkstdout(lulog **log, lulog_level threshold);
int lulog_mksyslog(lulog **log, const char *ident, lulog_level threshold);
int lulog_mkstring(lulog **log, lustr **string, lulog_level threshold);

int ludebug(lulog *log, const char *format, ...);
int luinfo(lulog *log, const char *format, ...);
int luwarn(lulog *log, const char *format, ...);
int luerror(lulog *log, const char *format, ...);

#endif
