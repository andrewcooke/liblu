
#ifndef LU_FILES_H
#define LU_FILES_H

#include <stdio.h>
#include "log.h"
#include "strings.h"

/**
 * @file
 *
 * @brief Various utilities for accessing files.
 */

/// Check if file exists (return value is check - no status).
int lufle_exists(lulog *log, const char *path);
/// Open a file (with error handling).
int lufle_open(lulog *log, const char *path, const char *mode, FILE **file);
/// Read a file.
int lufle_read(lulog *log, const char *path, lustr *contents);
/// Find a file (typically, a configuration file) using environment then directory and sub-directory.
/**
 * @param[in] log Destination for error messages.
 * @param[in] datadir Directory to check (if non-NULL and no `subdir`).
 * @param[in] subdir Sub-directory of `datadir` to check (if `datadir` and non-NULL).
 * @param[in] filename The filename to check.
 * @param[in] varname An environment variable name that may give a directory (if non-NULL).
 * @param[out] path The location of any file found.
 * @return LU_OK if the file is found.
 */
int lufle_find_config(lulog *log, const char *datadir, const char *subdir, const char *filename,
        const char *varname, lustr *path);
/// Find and then read a file (see `lufle_find_config()`).
int lufle_find_and_read_config(lulog *log, const char *datadir, const char *subdir,
		const char *filename, const char *varname, lustr *contents);

#endif
