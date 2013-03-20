#pragma once

#ifdef PIXELBOOST_LIBRARY_USE_GWEN

#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"

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
    class GwenInputHandler : public KeyboardHandler, public MouseHandler
    {
    public:
        GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root);
        ~GwenInputHandler();
        
    private:
        int GetInputHandlerPriority();
        bool OnKeyboardEvent(KeyboardEvent event);
        bool OnMouseEvent(MouseEvent event);
        
        glm::vec2 _PrevMouse;
        
        Gwen::Controls::Base* _Root;
        Gwen::Controls::Canvas*	_Canvas;
    };
}

#endif
