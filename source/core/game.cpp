#include "pixelboost/logic/scene.h"

#include "core/game.h"
#include "screens/mainScreen.h"

namespace pb
{
    Engine* Engine::Create(void* platformContext, int argc, const char** argv)
    {
        return new Game(platformContext, argc, argv);
    }
}

Game::Game(void* viewController, int argc, const char** argv)
    : pb::Engine(viewController, argc, argv, false)
{
    _MainScreen = new MainScreen();
    _MainScreen->SetActive(true);
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
    _MainScreen->Update(timeDelta, gameDelta);
    
    Engine::Update(timeDelta, gameDelta);
}

MainScreen* Game::GetMainScreen()
{
    return _MainScreen;
}
