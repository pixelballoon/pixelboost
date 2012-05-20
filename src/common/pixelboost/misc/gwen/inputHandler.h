#pragma once

#ifndef PIXELBOOST_DISABLE_GWEN

#include "pixelboost/input/touchManager.h"

namespace Gwen
{
    namespace Controls
    {
        class Canvas;
    }
}

namespace pb
{
    class GwenInputHandler : public TouchHandler
    {
    public:
        GwenInputHandler(Gwen::Controls::Canvas* canvas);
        ~GwenInputHandler();
        
    private:
        virtual void OnTouchBegin(Touch* touch);
        virtual void OnTouchUpdate(Touch* touch);
        virtual void OnTouchEnd(Touch* touch);
        
        Gwen::Controls::Canvas*	_Canvas;
        
        Vec2 _PrevMouse;
    };
}

#endif