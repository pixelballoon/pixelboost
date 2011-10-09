#include "libpixel/audio/soundManager.h"
#include "libpixel/debug/debugNetworkManager.h"
#include "libpixel/debug/debugVariableManager.h"
#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/render/font/fontRenderer.h"
#include "libpixel/graphics/render/model/modelRenderer.h"
#include "libpixel/graphics/render/primitive/primitiveRenderer.h"
#include "libpixel/graphics/render/sprite/spriteRenderer.h"
#include "libpixel/input/touchManager.h"
#include "libpixel/logic/game.h"
#include "libpixel/logic/screen.h"
#include "libpixel/network/gameCenter.h"
#include "libpixel/resource/resourceManager.h"

namespace libpixel
{

Game* Game::_Instance = 0;

Game::Game(void* viewController)
    : _GameTime(0)
    , _TotalTime(0)
    , _ViewController(viewController)
{
    _Instance = this;
    
#ifndef LIBPIXEL_DISABLE_DEBUG
    _DebugNetworkManager = new DebugNetworkManager();
#endif

    _ResourceManager = new ResourceManager();
    _TouchManager = new TouchManager();
    
    _GameCenter = new GameCenter();
    
    _GraphicsDevice = GraphicsDevice::Create();
    _FontRenderer = new FontRenderer();
    _ModelRenderer  = new ModelRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    _SpriteRenderer = new SpriteRenderer();
    
    _SoundManager = new SoundManager();

#ifndef LIBPIXEL_DISABLE_DEBUG
    _DebugNetworkManager->RegisterHandler(DebugVariableManager::Instance());

    _DebugNetworkManager->StartServer(6996, 1);
#endif
    
#ifndef LIBPIXEL_DISABLE_GAMECENTER
    _GameCenter->Connect();
#endif
}

Game::~Game()
{
#ifndef LIBPIXEL_DISABLE_DEBUG
    _DebugNetworkManager->StopServer();
    
    delete DebugVariableManager::Instance();
#endif

    _Instance = 0;
	
#ifndef LIBPIXEL_DISABLE_DEBUG
    delete _DebugNetworkManager;
#endif

    delete _FontRenderer;
    delete _GameCenter;
    delete _ModelRenderer;
    delete _PrimitiveRenderer;
    delete _ResourceManager;
    delete _SoundManager;
    delete _SpriteRenderer;
    delete _TouchManager;
}

Game* Game::Instance()
{
    return _Instance;
}

void Game::Initialise()
{

}

#ifndef LIBPIXEL_DISABLE_DEBUG
DebugNetworkManager* Game::GetDebugNetworkManager() const
{
    return _DebugNetworkManager;
}
#endif
    
FontRenderer* Game::GetFontRenderer() const
{
    return _FontRenderer;
}
    
GameCenter* Game::GetGameCenter() const
{
    return _GameCenter;
}
    
ModelRenderer* Game::GetModelRenderer() const
{
    return _ModelRenderer;
}

ResourceManager* Game::GetResourceManager() const
{
    return _ResourceManager;
}

PrimitiveRenderer* Game::GetPrimitiveRenderer() const
{
    return _PrimitiveRenderer;
}

SoundManager* Game::GetSoundManager() const
{
	return _SoundManager;
}
    
SpriteRenderer* Game::GetSpriteRenderer() const
{
    return _SpriteRenderer;
}

TouchManager* Game::GetTouchManager() const
{
    return _TouchManager;
}
    
float Game::GetGameTime()
{
    return _GameTime;
}

float Game::GetTotalTime()
{
    return _TotalTime;
}
    
bool Game::IsLandscape()
{
    return false;
}

void Game::Update(float time)
{
    _GameTime += time;
    _TotalTime += time;
    
#ifndef LIBPIXEL_DISABLE_DEBUG
    _DebugNetworkManager->Update();
#endif
}

void Game::Render()
{
}

void* Game::GetViewController()
{
    return _ViewController;
}

}
