#pragma once

#include "pixelboost/framework/engine.h"

class MainScreen;

class Game : public pb::Engine
{
public:
    Game(void* viewController, int argc, const char** argv);
    ~Game();
    
    static Game* Instance();
    
    virtual void Initialise();
    
    virtual void Update(float timeDelta, float gameDelta);
    
    MainScreen* GetMainScreen();
    
private:
    MainScreen* _MainScreen;
};
