
#ifndef LU_LOG_H
#define LU_LOG_H

#include <stdio.h>
#include <stdarg.h>

#include "strings.h"

/**
 * @file
 *
 * An extensible logging facility.  New `lulog` instances are created by
 * the `lulog_mk...` functions, which provide different implementations (to
 * a file, stderr, a string, etc).  The user can provide their own
 * implementation by returning a `lulog` instance with `lulog_vprintf` and
 * `lulog_free` functions.
 *
 * Once a `lulog` instance is available, functions like `ludebug`, `luinfo`,
 * etc can be used to log messages at the appropriate level.
 */
/// @{

/// The different severity levels for log messages (not needed for most use, via `ludebug` etc).
typedef enum {
	lulog_level_error,  ///< An error (likely fatal) has occurred.
	lulog_level_warn,  ///< The user should be aware of this abnormal condition.
	lulog_level_info,  ///< The user may be interested in this information.
	lulog_level_debug  ///< A developer may use this information to diagnose a problem.
} lulog_level;

struct lulog;
/// The interface for printing log messages that any implementation must provide.
typedef int lulog_vprintf(struct lulog *log, lulog_level level, const char *format, va_list ap);
/// The interface for freeing the log struct that any implementation must provide.
typedef int lulog_free(struct lulog **log, int status);

/// Any log implementation must provide an instance of this struct with appropriate members.
typedef struct lulog {
	lulog_level threshold;  ///< Only messages of this severity, or above, will be logged.
	lulog_vprintf *printfv;  ///< The implementation's print function.
	lulog_free *free;  ///< The implementation's free function.
	void *state;   ///< Opaque state available for use by the implementation.
} lulog;

/// Create a new `lulog` instance that logs to a `FILE`.
int lulog_mkstream(lulog **log, FILE *stream, lulog_level threshold, int close);
/// Create a new `lulog` instance that logs to `stderr`.
int lulog_mkstderr(lulog **log, lulog_level threshold);
/// Create a new `lulog` instance that logs to `stdout`.
int lulog_mkstdout(lulog **log, lulog_level threshold);
/// Create a new `lulog` instance that logs to syslog.
int lulog_mksyslog(lulog **log, const char *ident, lulog_level threshold);
/// Create a new `lulog` instance that logs to a `lustr`.
int lulog_mkstring(lulog **log, lustr **string, lulog_level threshold);

/// Log a (single line) message at the debug level.
int ludebug(lulog *log, const char *format, ...);
/// Log a (single line) message at the info level.
int luinfo(lulog *log, const char *format, ...);
/// Log a (single line) message at the warn level.
int luwarn(lulog *log, const char *format, ...);
/// Log a (single line) message at the error level.
int luerror(lulog *log, const char *format, ...);

/// Log multiple lines at the given level.
int lulog_lines(lulog *log, lulog_level level, const char *lines);
/// Log a (single line) message at the given level.
int lulog_printf(lulog *log, lulog_level level, const char *format, ...);

/// @}
#endif
