
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lu/status.h"
#include "lu/files.h"
#include "lu/log.h"


int lufle_exists(lulog *log, const char *path) {
	struct stat s = {};
	stat(path, &s);
	return S_ISREG(s.st_mode);
}

int lufle_find(lulog *log, const char *datadir, const char *subdir, const char *filename,
		const char *varname, lustr *path) {
	LU_STATUS
	lustr_mk(log, path);
	if (varname) {
		const char *var = getenv(varname);
		if (var) {
			lustr_printf(log, path, "%s/%s/%s", var, subdir, filename);
			if (lufle_exists(log, path->c)) {
				ludebug(log, "Found %s via %s at %s", filename, varname, path->c);
				goto exit;
			}
			LU_CHECK(lustr_clear(log, path))
		}
	}
	if (datadir) {
		lustr_printf(log, path, "%s/%s/%s", datadir, subdir, filename);
		if (lufle_exists(log, path->c)) goto exit;
		LU_CHECK(lustr_clear(log, path))
	}
	ludebug(log, "Could not find %s via %s, %s", filename, varname, datadir);
	LU_CHECK(lustr_free(path, status))
	status = LU_ERR_IO;
	LU_NO_CLEANUP
}

int lufle_read(lulog *log, const char *path, lustr *contents) {
	LU_STATUS
	FILE * f;
	LU_ASSERT(f = fopen(path, "r"), LU_ERR_IO, log, "Could not open %s", path)
	LU_ASSERT(!fseek(f, 0, SEEK_END), LU_ERR_IO, log, "Could not seek %s", path)
	long int fsize = ftell(f);
	rewind(f);
	LU_CHECK(lustr_mkn(log, contents, fsize+1))
	size_t read = fread(contents->c, 1, fsize, f);
	contents->mem.used = read + 1;
	LU_ASSERT(read == fsize, LU_ERR_IO, log, "Could not read %s", path)
	LU_NO_CLEANUP
}

int lufle_find_and_read(lulog *log, const char *datadir, const char *subdir, const char *filename,
		const char *varname, lustr *contents) {
	LU_STATUS
	lustr path = {};
	LU_CHECK(lufle_find(log, datadir, subdir, filename, varname, &path))
	LU_CHECK(lufle_read(log, path.c, contents))
	LU_NO_CLEANUP
}

