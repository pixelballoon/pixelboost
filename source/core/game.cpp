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
