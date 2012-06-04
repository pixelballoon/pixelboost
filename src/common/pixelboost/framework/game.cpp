#include "pixelboost/audio/soundManager.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/framework/game.h"
#include "pixelboost/framework/screen.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/custom/customRenderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/network/gameCenter.h"
#include "pixelboost/network/networkServer.h"

using namespace pb;

Game* Game::_Instance = 0;

Game::Game(void* viewController)
    : _GameTime(0)
    , _TotalTime(0)
    , _ViewController(viewController)
{
    _Instance = this;
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _TouchManager = new TouchManager();
    
    _Renderer = new Renderer();
    //_CustomRenderer = new CustomRenderer();
    //_ModelRenderer  = new ModelRenderer();
    //_ParticleRenderer = new ParticleRenderer();
    //_SpriteRenderer = new SpriteRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    //_FontRenderer = new FontRenderer();
#endif
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    _GameCenter = new GameCenter();
    _GameCenter->Connect();
#endif
    
#ifndef PIXELBOOST_DISABLE_INPUT
    _KeyboardManager = new KeyboardManager();
    _MouseManager = new MouseManager();
#endif
    
#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork = new NetworkServer();
    _DebugDatabaseHandler = new DebugDatabaseHandler();
    _DebugNetwork->RegisterHandler(_DebugDatabaseHandler);
#endif
}

Game::~Game()
{
    _Instance = 0;
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    delete _GameCenter;
#endif
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    delete _CustomRenderer;
    delete _FontRenderer;
    delete _ModelRenderer;
    delete _ParticleRenderer;
    delete _PrimitiveRenderer;
    delete _Renderer;
    delete _SpriteRenderer;
    
    delete _TouchManager;
#endif
}

Game* Game::Instance()
{
    return _Instance;
}

void Game::Initialise()
{
#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork->StartServer(9090);
#endif
}
    
CustomRenderer* Game::GetCustomRenderer() const
{
    return _CustomRenderer;
}
    
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
    
ParticleRenderer* Game::GetParticleRenderer() const
{
    return _ParticleRenderer;
}

PrimitiveRenderer* Game::GetPrimitiveRenderer() const
{
    return _PrimitiveRenderer;
}
    
Renderer* Game::GetRenderer() const
{
    return _Renderer;
}

SpriteRenderer* Game::GetSpriteRenderer() const
{
    return _SpriteRenderer;
}

KeyboardManager* Game::GetKeyboardManager() const
{
    return _KeyboardManager;
}

MouseManager* Game::GetMouseManager() const
{
    return _MouseManager;
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

glm::vec2 Game::GetScreenResolution()
{
    return glm::vec2(0,0);
}
    
void Game::OnMemoryWarning()
{
    
}

void Game::Update(float time)
{
    _GameTime += time;
    _TotalTime += time;
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer->Update(time);
#endif
    
#ifndef PIXELBOOST_DISABLE_SOUND
    SoundManager::Instance()->Update(time);
#endif
    
#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork->Update();
#endif
}

void Game::Render()
{
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer->Render();
#endif
}

void* Game::GetViewController()
{
    return _ViewController;
}
