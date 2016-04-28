
#ifndef LU_STRING_H
#define LU_STRING_H

#include <stdarg.h>

#include "lumem.h"

typedef struct lustr {
	char *c;
	lumem mem;
} lustr;

int lustr_mk(lulog *log, lustr *str);
int lustr_mkstr(lulog *log, lustr *str, const char *c);
int lustr_free(lulog *log, lustr *str, int status);

int lustr_printf(lulog *log, lustr *str, const char *format, ...);
int lustr_nprintf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_appendf(lulog *log, lustr *str, const char *format, ...);
int lustr_nappendf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_appendfv(lulog *log, lustr *str, const char *format, va_list ap);
int lustr_nappendfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap);
int lustr_truncate(lulog *log, lustr *str, int max_size);

#endif
