#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/game.h"
#include "pixelboost/logic/screen.h"

namespace pixelboost
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