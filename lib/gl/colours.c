
#include <math.h>
#include <string.h>

#include "lu/gl/colours.h"


// the code below is formatted assuming that when multiplying
// matrices and vectors, the vectors are columns.  the actual
// memory layout depends only on lumat_idx4.

// the aim is to (1) follow the usual conventions when
// writing matrix and vector maths and (2) have a memory
// layout consistent with opengl.

// (if you're looking for a general matrix / vector library this
// probably isn't it - it is very much intended to support
// opengl, hence the layout and the use of a fourth (w)
// component.


// memory convention here is that objects are allocated on the
// stack by the caller:
//   lumat_f4 m = {}; luvec_f4 v = {};
// and then passed to these routines using '&'.  this avoids
// tedious messing around with heap memory (but has consequences
// for stack use, obviously...)


void luglc_copy(luglc *a, luglc *b) {
    memcpy(b, a, sizeof(*a));
}

