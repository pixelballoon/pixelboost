#pragma once

#include "pixelboost/framework/game.h"

namespace pb
{
    class OrthographicCamera; 
    class RenderLayer;
    class Scene;
    class Viewport;
}

class Game : public pb::Game
{
public:
    Game(void* viewController);
    ~Game();
    
    virtual void Update(float time);
    virtual void Render();
    
private:
    pb::OrthographicCamera* _CameraA;
    pb::Viewport* _ViewportA;
    pb::OrthographicCamera* _CameraB;
    pb::Viewport* _ViewportB;
    
    pb::RenderLayer* _Layer;
    
    pb::Scene* _Scene;
};
