
#include <math.h>

#include "lu/vectors.h"


int lueq4(ludata_fxyzw a, ludata_fxyzw b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

ludata_fxyzw luadd3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){a.x+b.x, a.y+b.y, a.z+b.z, a.w};
}

ludata_fxyzw lusub3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){a.x-b.x, a.y-b.y, a.z-b.z, a.w};
}

ludata_fxyzw lucross3(ludata_fxyzw a, ludata_fxyzw b) {
    return (ludata_fxyzw){(a.y*b.z)-(a.z*b.y), -(a.x*b.z)+(a.z*b.x), (a.x*b.y)-(a.y*b.x), a.w};
}

float lulen3(ludata_fxyzw a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

ludata_fxyzw lunorm3(ludata_fxyzw a) {
    float l = lulen3(a);
    return (ludata_fxyzw){a.x/l, a.y/l, a.z/l, a.w};
}

ludata_fxyzw lusetw(ludata_fxyzw a, float w) {
    return (ludata_fxyzw){a.x, a.y, a.z, w};
}
