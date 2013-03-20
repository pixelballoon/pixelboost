#include <assert.h>

#include "pixelboost/debug/assert.h"

#ifndef PIXELBOOST_DISABLE_DEBUG
void PbAssert(int value)
{
    assert(value);
}
#else
#define PbAssert(v) do {} while(false);
#endif
