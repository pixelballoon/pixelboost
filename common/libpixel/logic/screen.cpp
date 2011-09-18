#include "libpixel/input/touchManager.h"
#include "libpixel/logic/game.h"
#include "libpixel/logic/screen.h"

namespace libpixel
{

Screen::Screen(Game* game)
    : _Game(game)
{
    
}

Screen::~Screen()
{
    delete _TouchHandler;
}

void Screen::Update(float time)
{
}

void Screen::Render()
{
}
    
void Screen::SetTouchActive()
{
    _Game->GetTouchManager()->SetHandler(_TouchHandler);
}
    
void Screen::SetTouchHandler(TouchHandler* touchHandler)
{
    _TouchHandler = touchHandler;
}

TouchHandler* Screen::GetTouchHandler()
{
    return _TouchHandler;
}
    
Game* Screen::GetGame()
{
    return _Game;
}

}