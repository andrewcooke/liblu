
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lu/internal.h"
#include "lu/files.h"
#include "lu/log.h"


int lufle_exists(lulog *log, const char *path) {
	struct stat s = {};
	stat(path, &s);
	int found = S_ISREG(s.st_mode);
	ludebug(log, "%s %s", found ? "Found" : "Could not find", path);
	return found;
}

int lufle_open(lulog *log, const char *path, const char *mode, FILE **file) {
    int status = LU_OK;;
    assert(*file = fopen(path, mode), LU_ERR_IO, log, "Could not open %s", path)
    ludebug(log, "Opened %s (%s)", path, mode);
    finally:
	return status;
}

int lufle_find_config(lulog *log, const char *datadir, const char *subdir, const char *filename,
		const char *varname, lustr *path) {
	int status = LU_OK;
	try(lustr_mk(log, path))
	if (varname) {
		const char *varvalue = getenv(varname);
		if (varvalue) {
			ludebug(log, "%s = %s", varname, varvalue);
			lustr_printf(log, path, "%s/%s", varvalue, filename);
			if (lufle_exists(log, path->c)) {
				ludebug(log, "Found %s via %s at %s", filename, varname, path->c);
				goto finally;
			}
			try(lustr_clear(log, path))
		}
	}
	if (datadir) {
	    if (subdir) {
            lustr_printf(log, path, "%s/%s/%s", datadir, subdir, filename);
	    } else {
	        lustr_printf(log, path, "%s/%s", datadir, filename);
	    }
		if (lufle_exists(log, path->c)) goto finally;
		try(lustr_clear(log, path))
	}
	luerror(log, "Could not find %s via %s, %s/%s", filename, varname, datadir, subdir);
	try(lustr_free(path, status))
	status = LU_ERR_IO;
	finally:
	return status;
}

int lufle_read(lulog *log, const char *path, lustr *contents) {
	int status = LU_OK;
	FILE *f;
	assert(f = fopen(path, "r"), LU_ERR_IO, log, "Could not open %s", path)
	assert(!fseek(f, 0, SEEK_END), LU_ERR_IO, log, "Could not seek %s", path)
	long int fsize = ftell(f);
	rewind(f);
	try(lustr_mkn(log, contents, fsize+1))
	size_t read = fread(contents->c, 1, fsize, f);
	contents->mem.used = read + 1;
	assert(read == fsize, LU_ERR_IO, log, "Could not read %s", path)
	finally:
	return status;
}

int lufle_find_and_read_config(lulog *log, const char *datadir, const char *subdir, const char *filename,
		const char *varname, lustr *contents) {
	int status = LU_OK;
	lustr path = {};
	try(lufle_find_config(log, datadir, subdir, filename, varname, &path))
	try(lufle_read(log, path.c, contents))
	finally:
	return status;
}

