#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

#include "core/game.h"
#include "screens/gameScreen.h"
#include "screens/menuScreen.h"

namespace pb
{
    Engine* Engine::Create(void* platformContext, int argc, const char** argv)
    {
        return new Game(platformContext, argc, argv);
    }
}

Game::Game(void* viewController, int argc, const char** argv)
    : pb::Engine(viewController, argc, argv)
    , _GameMode(kGameModeUninitialised)
{
    _GameScreen = new GameScreen();
    _MenuScreen = new MenuScreen();
    
    SetMode(kGameModeMenu);
}

Game::~Game()
{

}

Game* Game::Instance()
{
    return static_cast<Game*>(pb::Engine::Instance());
}

void Game::Update(float timeDelta, float gameDelta)
{
    switch (_GameMode)
    {
        case kGameModeGame:
            _GameScreen->Update(timeDelta, gameDelta);
            break;
            
        case kGameModeMenu:
            _MenuScreen->Update(timeDelta, gameDelta);
            
        case kGameModeUninitialised:
            break;
    }
    
    Engine::Update(timeDelta, gameDelta);
}

void Game::SetMode(GameMode gameMode)
{
    switch (_GameMode)
    {
        case kGameModeGame:
            _GameScreen->SetActive(false);
            break;
        case kGameModeMenu:
            _MenuScreen->SetActive(false);
            break;
        case kGameModeUninitialised:
            break;
    }
    
    _GameMode = gameMode;
    
    switch (_GameMode)
    {
        case kGameModeGame:
            _GameScreen->SetActive(true);
            break;
        case kGameModeMenu:
            _MenuScreen->SetActive(true);
            break;
        case kGameModeUninitialised:
            PbAssert(!"You shouldn't be setting game mode to this...");
            break;
    }
}

GameScreen* Game::GetGameScreen()
{
    return _GameScreen;
}

MenuScreen* Game::GetMenuScreen()
{
    return _MenuScreen;
}
