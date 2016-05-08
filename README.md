
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

## Simplex Noise

An efficient implementation based on code from Stefan Gustavson and
Peter Eastman.

```c
#include "lusimplex.h"
noise = lusimplex_noise2(x, y);
```

## Greyscale

Support for greyscale images (output from 
[check_simplex](../blob/master/tests/check_simplex.c)):

```
 :++:.:...:++++:::+:***+:..:++*oOO#@@OOo
 .+*+::...:::+::.:+***+::..::+:+oooO#OOo
 .::+*::..::+:+:::*o**::+:::++:+**oOOOo*
.:+*++:.:::****++*ooo*++++++::::::+*oo*:
*+***+:.+**o**oooo*o****oo++:::.::++**++
Ooo**:.::**ooo**oooo**o*Ooo*+:.:++:+*o*+
OOo***+++***oo**oOOOooOOOo**+:.:::++*Oo+
oOO*+**+ooooO***O#@#OO#@@Oo*++:::+++oOO*
*ooo****ooOOo**oO###OO@@###oo****:++oO#o
oo*oOoooooOoo**ooo####@@#@@O*+***++oO#@O
oo*oOOOoooOo*++**oOoOO#@@@#O*+++***oO##O
*ooOOOOOOOOo+::::*ooOOO#@@#o+:+****OOOOO
+O##@###Ooo+:....+ooooOO@@Oo+:+****oooO#
*O@@@@#OOo*+::. .+*oooOOO#oo+:+****oOO##
oO@@@@OOoo*+::..:+**oOO#@#OO*++****oOO##
O#@@###OOoo*+:++**oooo#@@#Oo*++**o*oO#O#
oO#####OOOOo*+*ooooOOo###Ooo+++++*ooOOOO
**oOO###OOo**+ooOOO##OOOOO++:++:+*ooOo**
+++oOOO#OOo*++oO#@@##OOo**::++++*oOOOo+*
::+**ooOOO****oO#@@##Oo*+::+*o*ooO@#O*++
```

## Conventions

Most routines return an `int` status that is non-zero on error.  Free
routines take the same value to allow chaining on cleanup.  See source
for how this is used in practice.

Most routines take a `lulog*` pointer as the first argument.  This can
be NULL if no logging is required.

Most names have the form `luLIB_NAME` (eg. `lustr_printf`), but the most
widely used are simply `luNAME` (eg. `lustr`, `ludebug`).
