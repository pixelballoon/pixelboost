#pragma once

#ifndef PIXELBOOST_DISABLE_GWEN

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
    class GwenInputHandler : public MouseHandler
    {
    public:
        GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root);
        ~GwenInputHandler();
        
        virtual int GetPriority();
        
        virtual bool OnMouseDown(MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnMouseScroll(glm::vec2 delta);
        
    private:
        Gwen::Controls::Base* _Root;
        Gwen::Controls::Canvas*	_Canvas;
        
        glm::vec2 _PrevMouse;
    };
}

#endif
