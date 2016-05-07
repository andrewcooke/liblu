
# Light Utility Library

## String Buffer

The `lustr` structure is intended to be used as a buffer where complex
strings can be assembled (it's a `char*` plus a couple of values needed for
bookkeeping memory):

```c
#include "lustr.h"
lustr str;
lustr_init(log, &str);
lustr_print(log, &str, "hello");
lustr_appendf(log, &str, " world %d", 42);
...
lustr_appendf(log, &str, format, ap);  // stdarg
fprintf(stdout, "%s", str.c);  // hello world 42 ...
lustr_free(log, &str, 0); 
```

The `log` above can be `NULL`, or an instance of the log below.

## Log

The `lulog` structure can log to a stream, file, syslog, or an in-memory
string:

```c
#include "lulog.h"
lulog *log;
lulog_mkstderr(&log, lulog_level_info);  // don't display debug
luinfo(log, "So far all OK");
luerror(log, "Bad thing: %d != %d", 2, 37);
...
lulog_free(&log, 0);  // or status = lulog_free(&log, status);
```

## Conventions

Most routines return an `int` status that is non-zero on error.  Free
routines take the same value to allow chaining on cleanup.  See source
for how this is used in practice.

Most routines take a `lulog*` pointer as the first argument.  This can
be NULL if no logging is required.

Most names have the form `luLIB_NAME` (eg. `lustr_printf`), but the most
widely used are simply `luNAME` (eg. `lustr`, `ludebug`).
