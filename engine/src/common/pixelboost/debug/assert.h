#pragma once

#ifndef PIXELBOOST_DISABLE_DEBUG
    #include <assert.h>
    #define PbAssert(value) assert(value)
#else
    #define PbAssert(value) do {} while(0);
#endif
