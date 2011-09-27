/*
    File:       LibSVL.cpp

    Purpose:    Compiles all code necessary for SVL.h.

    Author:     Andrew Willmott
*/

#define __SVL__

#ifdef VL_DEBUG
#define VL_CHECKING
#endif

#define LIBPIXEL__MATH__COMPILEGUARD

using namespace std;

#include "basics.h"
#include "constants.h"
#include "utils.h"

#include "vec4.cpp"
#include "vec.cpp"

#include "mat2.cpp"
#include "mat3.cpp"
#include "mat4.cpp"
#include "mat.cpp"

#undef LIBPIXEL__MATH__COMPILEGUARD