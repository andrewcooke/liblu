
#ifndef LU_LOG_H
#define LU_LOG_H

#include <stdio.h>
#include <stdarg.h>

#include "strings.h"

/**
 * @defgroup lu_log Logging.
 * @ingroup lu
 *
 * An extensible logging facility.
 */
/// @{

/// The different severity levels for log messages.
typedef enum lulog_level {
	lulog_level_error,  ///< An error (likely fatal) has occurred.
	lulog_level_warn,  ///< The user should be aware of this abnormal condition.
	lulog_level_info,  ///< The user may be interested in this information.
	lulog_level_debug  ///< A developer may use this information to diagnose a problem.
} lulog_level;

struct lulog;
typedef int lulog_vprintf(struct lulog *log, lulog_level level, const char *format, va_list ap);
typedef int lulog_free(struct lulog **log, int status);

/// @public
/// jiojd
typedef struct lulog {
	lulog_level threshold;  ///< dsa
	lulog_vprintf *printfv;
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

int lulog_lines(lulog *log, lulog_level level, const char *lines);
int lulog_printf(lulog *log, lulog_level level, const char *format, ...);

/// @}
#endif
