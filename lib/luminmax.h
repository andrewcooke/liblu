
#ifndef LU_MINMAX_H
#define LU_MINMAX_H

// http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a >= _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a <= _b ? _a : _b; })

#endif
