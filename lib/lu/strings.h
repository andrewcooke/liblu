
#ifndef LU_STRINGS_H
#define LU_STRINGS_H

#include <stdarg.h>

#include "dynamic_memory.h"

typedef struct lulog lulog;  // avoid dependency loops

typedef struct lustr {
	char *c;
	lumem mem;
} lustr;

int lustr_mk(lulog *log, lustr *str);
int lustr_mkn(lulog *log, lustr *str, size_t n);
int lustr_mkstr(lulog *log, lustr *str, const char *c);
int lustr_free(lustr *str, int status);
int lustr_reserve(lulog *log, lustr *str, int n);
int lustr_clear(lulog *log, lustr *str);

int lustr_add(lulog *log, lustr *str, char c);
int lustr_sprint(lulog *log, lustr *str, const char *text);
int lustr_nsprint(lulog *log, lustr *str, int max_size, const char *text);
int lustr_sprintf(lulog *log, lustr *str, const char *format, ...);
int lustr_nsprintf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_vsprintf(lulog *log, lustr *str, const char *format, va_list ap);
int lustr_vnsprintf(lulog *log, lustr *str, int max_size, const char *format, va_list ap);
int lustr_append(lulog *log, lustr *str, const char *text);
int lustr_nappend(lulog *log, lustr *str, int max_size, const char *text);
int lustr_appendf(lulog *log, lustr *str, const char *format, ...);
int lustr_nappendf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_vappendf(lulog *log, lustr *str, const char *format, va_list ap);
int lustr_vnappendf(lulog *log, lustr *str, int max_size, const char *format, va_list ap);

#endif
