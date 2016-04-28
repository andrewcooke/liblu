
// definitions used by all components.

#ifndef LU_H
#define LU_H

#define LU_OK 0
#define LU_ERR 1
#define LU_ERR_MEM 2
#define LU_ERR_IO 3

// http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a >= _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a <= _b ? _a : _b; })

#endif
