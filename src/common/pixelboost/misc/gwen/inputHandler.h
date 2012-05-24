#pragma once

#ifndef PIXELBOOST_DISABLE_GWEN

namespace Gwen
{
    namespace Controls
    {
        class Base;
        class Canvas;
    }
}

namespace pb
{
    class GwenMouseHandler;
    class GwenKeyboardHandler;
    
    class GwenInputHandler
    {
    public:
        GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root);
        ~GwenInputHandler();
        
    private:
        GwenKeyboardHandler* _KeyboardHandler;
        GwenMouseHandler* _MouseHandler;
    };
}

#endif
