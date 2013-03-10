#pragma once

#include "pixelboost/framework/screen.h"

namespace pb
{
    class OrthographicCamera;
    class Scene;
    class Viewport;
}

class MenuInputHandler;

class MenuScreen : public pb::Screen
{
public:
    MenuScreen();
    ~MenuScreen();
    
    virtual void Update(float timeDelta, float gameDelta);
    
    virtual void SetActive(bool active);
    
private:
    void AddControls();
    
    MenuInputHandler* _InputHandler;
    
    pb::OrthographicCamera* _Camera;
    pb::Scene* _Scene;
    pb::Viewport* _Viewport;
};
