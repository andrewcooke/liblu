
#ifndef LU_LOG_H
#define LU_LOG_H

#include <stdio.h>

typedef enum lulog_level {
	lulog_level_debug,
	lulog_level_info,
	lulog_level_warn,
	lulog_level_error,
	lulog_level_fatal
} lulog_level;

struct lulog;
typedef lulog_print(struct lulog *log, const char *format, ...);
typedef lulog_free(struct lulog **log, int status);

typedef struct lulog {
	lulog_print *debug;
	lulog_print *info;
	lulog_print *warn;
	lulog_print *error;
	lulog_print *fatal;
	lulog_free *free;
	void *state;
} lulog;

int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold);
int lulog_mkstderr(lulog **log, lulog_level threshold);
int lulog_mkstdout(lulog **log, lulog_level threshold);

#endif
