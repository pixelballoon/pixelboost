#pragma once

#ifndef PIXELBOOST_DISABLE_DEBUG
    void PbAssert(int value);
#else
    #define PbAssert(a) do {} while(0);
#endif
