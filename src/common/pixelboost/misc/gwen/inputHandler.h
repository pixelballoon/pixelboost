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
        bool OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character);
        bool OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character);
        bool OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        bool OnMouseMove(glm::vec2 position);
        bool OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        bool OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 scroll);
        bool OnMouseZoom(glm::vec2 zoom);
        bool OnMouseRotate(float rotate);
        
        glm::vec2 _PrevMouse;
        
        Gwen::Controls::Base* _Root;
        Gwen::Controls::Canvas*	_Canvas;
    };
}

#endif
