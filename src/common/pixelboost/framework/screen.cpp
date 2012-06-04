#include "pixelboost/framework/game.h"
#include "pixelboost/framework/screen.h"
#include "pixelboost/input/touchManager.h"

namespace pb
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
    
void Screen::SetActive(bool active)
{
    
}
    
Game* Screen::GetGame()
{
    return _Game;
}

}
