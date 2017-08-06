
#ifndef LU_FILES_H
#define LU_FILES_H

#include <stdio.h>
#include "log.h"
#include "strings.h"

int lufle_exists(lulog *log, const char *path);
int lufle_open(lulog *log, const char *path, const char *mode, FILE **file);
int lufle_find(lulog *log, const char *datadir, const char *subdir, const char *filename,
		const char *varname, lustr *path);
int lufle_read(lulog *log, const char *path, lustr *contents);
int lufle_find_and_read(lulog *log, const char *datadir, const char *subdir,
		const char *filename, const char *varname, lustr *contents);

#endif
