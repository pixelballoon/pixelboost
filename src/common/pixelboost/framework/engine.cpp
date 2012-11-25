#include "pixelboost/audio/soundManager.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/framework/screen.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
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

#include "pixelboost/debug/log.h"

using namespace pb;

Engine* Engine::_Instance = 0;

Engine::Engine(void* viewController)
    : _GameTime(0)
    , _TotalTime(0)
    , _ViewController(viewController)
{
    _Instance = this;

    _FileSystem = new FileSystem();

#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer = new Renderer();
    _BufferRenderer = new BufferRenderer();
    _ModelRenderer  = new ModelRenderer();
    _ParticleRenderer = new ParticleRenderer();
    _SpriteRenderer = new SpriteRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    _FontRenderer = new FontRenderer();
#endif

#ifndef PIXELBOOST_DISABLE_GAMECENTER
    _GameCenter = new GameCenter();
    _GameCenter->Connect();
#endif

#ifndef PIXELBOOST_DISABLE_INPUT
    _KeyboardManager = new KeyboardManager();
    _MouseManager = new MouseManager();
    _TouchManager = new TouchManager();
#endif

#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork = new NetworkServer();
    _DebugDatabaseHandler = new DebugDatabaseHandler();
    _DebugNetwork->RegisterHandler(_DebugDatabaseHandler);
#endif
}

Engine::~Engine()
{
    _Instance = 0;
    
    delete _FileSystem;
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    delete _GameCenter;
#endif
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    delete _BufferRenderer;
    delete _FontRenderer;
    delete _ModelRenderer;
    delete _ParticleRenderer;
    delete _PrimitiveRenderer;
	delete _SpriteRenderer;
    delete _Renderer;
#endif
}

Engine* Engine::Instance()
{
    return _Instance;
}

void Engine::Initialise()
{
#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork->StartServer(9090, 1);
#endif
}

GameCenter* Engine::GetGameCenter() const
{
    return _GameCenter;
}
    
BufferRenderer* Engine::GetBufferRenderer() const
{
    return _BufferRenderer;
}
    
FontRenderer* Engine::GetFontRenderer() const
{
    return _FontRenderer;
}

ModelRenderer* Engine::GetModelRenderer() const
{
    return _ModelRenderer;
}

ResourceManager* Engine::GetResourceManager() const
{
    return _ResourceManager;
}
    
ParticleRenderer* Engine::GetParticleRenderer() const
{
    return _ParticleRenderer;
}

PrimitiveRenderer* Engine::GetPrimitiveRenderer() const
{
    return _PrimitiveRenderer;
}

SpriteRenderer* Engine::GetSpriteRenderer() const
{
    return _SpriteRenderer;
}

KeyboardManager* Engine::GetKeyboardManager() const
{
    return _KeyboardManager;
}

MouseManager* Engine::GetMouseManager() const
{
    return _MouseManager;
}

TouchManager* Engine::GetTouchManager() const
{
    return _TouchManager;
}
    
float Engine::GetGameTime()
{
    return _GameTime;
}

float Engine::GetTotalTime()
{
    return _TotalTime;
}
    
bool Engine::IsLandscape()
{
    return false;
}

bool Engine::AllowFrameskip()
{
    return true;
}
    
void Engine::OnMemoryWarning()
{
    
}

void Engine::OnAppLoseFocus()
{

}

void Engine::OnAppGainFocus()
{

}

void Engine::OnAppQuit()
{

}

void Engine::Update(float time)
{
    _GameTime += time;
    _TotalTime += time;
    
#ifndef PIXELBOOST_DISABLE_SOUND
    SoundManager::Instance()->Update(time);
#endif
    
#ifndef PIXELBOOST_DISABLE_DEBUG
    _DebugNetwork->Update();
#endif
}

void Engine::Render()
{
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer->Render();
#endif
}

void* Engine::GetViewController()
{
    return _ViewController;
}
