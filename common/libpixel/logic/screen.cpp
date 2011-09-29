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
}

void Screen::Update(float time)
{
}

void Screen::Render()
{
}
    
Game* Screen::GetGame()
{
    return _Game;
}

}