#ifndef PIXELBOOST_DISABLE_GWEN

#include "gwen/Controls/Canvas.h"

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/game.h"
#include "pixelboost/misc/gwen/inputHandler.h"

using namespace pb;

GwenInputHandler::GwenInputHandler(Gwen::Controls::Canvas* canvas)
    : _Canvas(canvas)
{
    pb::Game::Instance()->GetTouchManager()->AddTouchHandler(this);
}

GwenInputHandler::~GwenInputHandler()
{
    pb::Game::Instance()->GetTouchManager()->RemoveTouchHandler(this);
}

void GwenInputHandler::OnTouchBegin(Touch* touch)
{
    Vec2 pos = touch->GetDisplayPosition();
    _PrevMouse = pos;
    
    _Canvas->InputMouseMoved(pos[0], pos[1], 0, 0);
    _Canvas->InputMouseButton( 0, true );
}

void GwenInputHandler::OnTouchUpdate(Touch* touch)
{
    Vec2 pos = touch->GetDisplayPosition();
    Vec2 delta = pos - _PrevMouse;
    _PrevMouse = touch->GetScreenPosition();

    _Canvas->InputMouseMoved(pos[0], pos[1], delta[0], delta[1]);
}

void GwenInputHandler::OnTouchEnd(Touch* touch)
{
    Vec2 pos = touch->GetDisplayPosition();
    Vec2 delta = pos - _PrevMouse;
    _PrevMouse = touch->GetScreenPosition();
    
    _Canvas->InputMouseMoved(pos[0], pos[1], delta[0], delta[1]);
    _Canvas->InputMouseButton(0, false);
}

#endif
