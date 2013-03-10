#pragma once

#include "pixelboost/framework/screen.h"

namespace pb
{
    class OrthographicCamera;
    class Scene;
    class Viewport;
}

class GameScreen : public pb::Screen
{
public:
    GameScreen();
    ~GameScreen();
    
    virtual void Update(float timeDelta, float gameDelta);
    
    virtual void SetActive(bool active);
    
private:
    pb::OrthographicCamera* _Camera;
    pb::Scene* _Scene;
    pb::Viewport* _Viewport;
};
