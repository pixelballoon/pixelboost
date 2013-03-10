#pragma once

#include "pixelboost/framework/engine.h"

enum GameMode
{
    kGameModeUninitialised,
    kGameModeGame,
    kGameModeMenu
};

class GameScreen;
class MenuScreen;

class Game : public pb::Engine
{
public:
    Game(void* viewController, int argc, const char** argv);
    ~Game();
    
    static Game* Instance();
    
    virtual void Update(float timeDelta, float gameDelta);
    
    void SetMode(GameMode gameMode);
    
    GameScreen* GetGameScreen();
    MenuScreen* GetMenuScreen();
    
private:
    GameScreen* _GameScreen;
    MenuScreen* _MenuScreen;
    
    GameMode _GameMode;
};
