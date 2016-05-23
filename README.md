
# Light Utility Library

A C99 library with few dependencies that contains whatever utilities I need.

## Table of Contents

  * [Log](#log)
  * [Dynamic Memory and Arrays](#dynamic-memory-and-arrays)
  * [String Buffer](#string-buffer)
  * [Random](#random)
  * [Greyscale](#greyscale)
  * [Simplex Noise](#simplex-noise)
  * [Tileable Simplex Noise](#tileable-simplex-noise)
  * [Conventions](#conventions)

## Log

The `lulog` structure can log to a stream, file, syslog, or an in-memory
string:

```c
#include "lu/log.h"
lulog *log;
lulog_mkstderr(&log, lulog_level_info);  // don't display debug
luinfo(log, "So far all OK");
luerror(log, "Bad thing: %d != %d", 2, 37);
...
lulog_free(&log, 0);  // or status = lulog_free(&log, status);
```

## Dynamic Memory and Arrays

Simple framework for dynamic memory management in `lu/dynamic_memory.h`, 
extended in `lu/array_macros.h`.

```c
#include "lu/arrays.h"
luarray_int *ints;
luarray_mkintn(log, &ints, 10);  // initial capacity 10
luarray_pushint(log, ints, 42);
luarray_freeint(&ints, 0);
``` 

## String Buffer

The `lustr` structure (using the dynamic memory framework) is intended to 
be used as a buffer where complex strings can be assembled:

```c
#include "lu/strings.h"
lustr str = {};
lustr_init(log, &str);
lustr_print(log, &str, "hello");
lustr_appendf(log, &str, " world %d", 42);
...
lustr_appendfv(log, &str, format, ap);  // stdarg
fprintf(stdout, "%s", str.c);  // hello world 42 ...
lustr_free(log, &str, 0); 
```

The `log` above can be `NULL`, or an instance of the log above.

## Random

The `lurand` structure is a source of random integers:

```c
#include "lu/random.h"
lurand *rand;
lurand_mkxoroshiro128plus(log, &rand, 0);
int n = lurand_int64_range(rand, lo, hi);
lurand_shuffle(log, rand, anagram, 1, strlen(anagram));
rand->free(rand, 0);
```

## Greyscale

See examples below.

## Simplex Noise

An efficient implementation based on code from Stefan Gustavson and
Peter Eastman.

```c
#include "lu/simplex.h"
noise = lusimplex_noise2(x, y);
```

## Tileable Simplex Noise

This is a rewrite of the 2D Gustavson code, making it more flexible 
(configurable size of gradients and permutations, tilings), and more correct 
(gradients are unit vectors), but also much slower. 

```c
#include "lu/tiles.h"
lutile_tile *hexagon;
lutile_mkhexagon(log, &hexagon, 3, 2, 1.0));
luarray_ijz *ijz = NULL;
hexagon->enumerate(hexagon, log, config, 7, &ijz);
```

When tiled three times:

```
                         :mboi+iociom*     
                      "+;cccccIBIc;=;c=    
                     =cccIBECCCIbecj;;;-   
                    c@Cc:,:cjijcIBIcebec=  
                   c@Em;,+;*=" ;CIbecIBEC; 
                  iBIc*"*cJMJcj;jcemeccccc 
       :mboi+iociomc;- .,:cJMMMmiiiomecob* 
    "+;cccccIBIc;=;cbCbc;=;cJMJc;=;cemec=  
   =cccIBECCCIbecj;;;icJ@ObCMJc*"' .,=+"   
  c@Cc:,:cjijcIBIcebecj;jcemeccccc:,:c=    
 c@Em;,+;*=" ;CIbecIBECmo+ "=;cC@EmIC;     
 BIc*"*cJMJcj;jcemecccccccccemec;+;c=      
 -;- .,:cJMMMmiiiomecob* :mboi+iociom*     
  ;Cbc;=;cJMJc;=;cemec;+;cccccIBIc;=;c=    
   =cJ@ObCMJc*"' .,=+;cccIBECCCIbecj;;;-   
    -;jcemeccccc:,:cC@Cc:,:cjijcIBIcebec=  
     +o+ "=;cC@EmICB@Em;,+;*=" ;CIbecIBEC; 
      =cccemec;+;cIBIc*"*cJMJcj;jcemeccccc 
                   -;- .,:cJMMMmiiiomecob* 
                    ;Cbc;=;cJMJc;=;cemec=  
                     =cJ@ObCMJc*"' .,=+"   
                      -;jcemeccccc:,:c=    
                       +o+ "=;cC@EmIC;     
                        =cccemec;+;c=     
```

## Conventions

Most routines return an `int` status that is non-zero on error.  Free
routines take the same value to allow chaining on cleanup.  See source
and `lustatus.h` for how this is used in practice.

Most routines take a `lulog*` pointer as the first argument.  This can
be NULL if no logging is required.

Most names have the form `luLIB_NAME` (eg. `lustr_printf`), but the most
widely used are simply `luNAME` (eg. `lustr`, `ludebug`).

Most argument lists are ordered: log, library structs, other input parameters,
other output parameters.
