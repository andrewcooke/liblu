
#ifndef LU_COLOURS_H
#define LU_COLOURS_H

#include "log.h"
#include "gl_types.h"


// all input arrays passed by pointer.
// operations that return void mutate a result argument
// (typically matrices).

void luglc_copy(luglc *a, luglc *b);

#endif
