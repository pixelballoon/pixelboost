#pragma once

#include "pixelboost/logic/game.h"

namespace pb
{
    class OrthographicCamera; 
    class RenderLayer;
}

class Game : public pb::Game
{
public:
    Game(void* viewController);
    ~Game();
    
    virtual void Update(float time);
    virtual void Render();
    
private:
    pb::OrthographicCamera* _Camera;
    pb::RenderLayer* _Layer;
};
