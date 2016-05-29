
#ifndef LU_VECTORS_H
#define LU_VECTORS_H

#include "structs.h"

int lueq4(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw luadd3(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw lusub3(ludata_fxyzw a, ludata_fxyzw b);
ludata_fxyzw lucross3(ludata_fxyzw a, ludata_fxyzw b);
float lulen3(ludata_fxyzw a);
ludata_fxyzw lunorm3(ludata_fxyzw a);

#endif
