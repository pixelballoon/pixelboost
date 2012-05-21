#pragma once

#include <string>

#include "pixelboost/logic/game.h"
#include "pixelboost/math/maths.h"
#include "pixelboost/misc/delegate/signal.h"

#include "manipulator/selection.h"

namespace Gwen
{
    namespace Controls
    {
        class Canvas;
    }
}

namespace pb
{
    class GwenInputHandler;
    class OrthographicCamera;
    class RenderLayer;
}

namespace pixeleditor
{

class CommandManager;
class HttpInterface;
class Project;

class View : public pb::Game
{
public:
    View();
    ~View();
    
public:
    static View* Instance();
    
    virtual void Initialise();
    
    virtual Vec2 GetScreenResolution();
    
    virtual void Render();
    
    void SetCanvasSize(Vec2 size);
    
private:
    Vec2 _CanvasSize;
    
    pb::GwenInputHandler* _GwenInput;
    pb::RenderLayer* _GwenLayer;
    pb::OrthographicCamera* _GwenCamera;
    Gwen::Controls::Canvas* _GwenCanvas;
};
    
}
