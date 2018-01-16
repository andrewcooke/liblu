
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

/// @brief Find a file (typically, a configuration file) using environment then directory and sub-directory.
/// @return LU_OK if the file is found.
int lufle_find_config(
		lulog *log,  ///< [in] Destination for error messages.
		const char *datadir,   ///< [in] Directory to check (if non-NULL and no `subdir`).
		const char *subdir,   ///< [in] Sub-directory of `datadir` to check (if `datadir` and non-NULL).
		const char *filename,  ///< [in] The filename to check.
        const char *varname,   ///< [in] An environment variable name that may give a directory (if non-NULL).
		lustr *path  ///< [out] The location of any file found.
		);

/// Find and then read a file (see `lufle_find_config()`).
int lufle_find_and_read_config(lulog *log, const char *datadir, const char *subdir,
		const char *filename, const char *varname, lustr *contents);

#endif
