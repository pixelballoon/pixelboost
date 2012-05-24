#ifndef PIXELBOOST_DISABLE_GWEN

#include "gwen/Controls/Canvas.h"

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/game.h"
#include "pixelboost/misc/gwen/inputHandler.h"

using namespace pb;

GwenInputHandler::GwenInputHandler(Gwen::Controls::Canvas* canvas, Gwen::Controls::Base* root)
    : _Canvas(canvas)
    , _Root(root)
{
    pb::Game::Instance()->GetMouseManager()->AddHandler(this);
}

GwenInputHandler::~GwenInputHandler()
{
    pb::Game::Instance()->GetMouseManager()->AddHandler(this);
}

int GwenInputHandler::GetPriority()
{
    return 1000;
}

bool GwenInputHandler::OnMouseDown(MouseButton button, glm::vec2 position)
{
    glm::vec2 delta(0,0);
    _PrevMouse = position;
    
    _Canvas->InputMouseMoved(position.x, position.y, delta[0], delta[1]);
    Gwen::Controls::Base* hoveredControl = Gwen::HoveredControl;
    _Canvas->InputMouseButton((int)button, true);
    if (hoveredControl == _Root)
        return false;
    
    return true;
}

bool GwenInputHandler::OnMouseMove(glm::vec2 position)
{
    glm::vec2 delta = position - _PrevMouse;
    _PrevMouse = position;

    return _Canvas->InputMouseMoved(position.x, position.y, delta[0], delta[1]);
}

bool GwenInputHandler::OnMouseUp(MouseButton button, glm::vec2 position)
{
    glm::vec2 delta = position - _PrevMouse;
    _PrevMouse = position;
    
    return _Canvas->InputMouseButton((int)button, false);
}

bool GwenInputHandler::OnMouseScroll(glm::vec2 scroll)
{
    return _Canvas->InputMouseWheel(scroll.y*120);
}

#endif
