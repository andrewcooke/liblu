
#ifndef LU_STRING_H
#define LU_STRING_H

#include <stdarg.h>

#include "lumem.h"


typedef struct lulog lulog;  // avoid dependency loops

typedef struct lustr {
	char *c;
	lumem mem;
} lustr;

int lustr_init(lulog *log, lustr *str);
int lustr_initstr(lulog *log, lustr *str, const char *c);
int lustr_free(lulog *log, lustr *str, int status);
int lustr_reserve(lulog *log, lustr *str, int n);
int lustr_clear(lulog *log, lustr *str);

int lustr_print(lulog *log, lustr *str, const char *text);
int lustr_nprint(lulog *log, lustr *str, int max_size, const char *text);
int lustr_printf(lulog *log, lustr *str, const char *format, ...);
int lustr_nprintf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_printfv(lulog *log, lustr *str, const char *format, va_list ap);
int lustr_nprintfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap);
int lustr_append(lulog *log, lustr *str, const char *text);
int lustr_nappend(lulog *log, lustr *str, int max_size, const char *text);
int lustr_appendf(lulog *log, lustr *str, const char *format, ...);
int lustr_nappendf(lulog *log, lustr *str, int max_size, const char *format, ...);
int lustr_appendfv(lulog *log, lustr *str, const char *format, va_list ap);
int lustr_nappendfv(lulog *log, lustr *str, int max_size, const char *format, va_list ap);

#endif
