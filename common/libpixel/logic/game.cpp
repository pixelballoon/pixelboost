#include "libpixel/audio/soundManager.h"
#include "libpixel/debug/debugVariableManager.h"
#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/render/common/renderer.h"
#include "libpixel/graphics/render/font/fontRenderer.h"
#include "libpixel/graphics/render/model/modelRenderer.h"
#include "libpixel/graphics/render/particle/particleRenderer.h"
#include "libpixel/graphics/render/primitive/primitiveRenderer.h"
#include "libpixel/graphics/render/sprite/spriteRenderer.h"
#include "libpixel/input/touchManager.h"
#include "libpixel/logic/game.h"
#include "libpixel/logic/screen.h"
#include "libpixel/network/gameCenter.h"
#include "libpixel/network/networkServer.h"
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

    _ResourceManager = new ResourceManager();
    _TouchManager = new TouchManager();
    
    _GameCenter = new GameCenter();
    
    _FontRenderer = new FontRenderer();
    _ModelRenderer  = new ModelRenderer();
    _ParticleRenderer = new ParticleRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    _Renderer = new Renderer();
    _SpriteRenderer = new SpriteRenderer();

    _Renderer->AddRenderer(_ParticleRenderer);
    _Renderer->AddRenderer(_PrimitiveRenderer);
    _Renderer->AddRenderer(_SpriteRenderer);
    
    _SoundManager = new SoundManager();
    
#ifndef LIBPIXEL_DISABLE_GAMECENTER
    _GameCenter->Connect();
#endif
}

Game::~Game()
{
    _Instance = 0;

    delete _FontRenderer;
    delete _GameCenter;
    delete _ModelRenderer;
    delete _ParticleRenderer;
    delete _PrimitiveRenderer;
    delete _Renderer;
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
    
    _Renderer->Update(time);
}

void Game::Render()
{
    _Renderer->Render();
}

void* Game::GetViewController()
{
    return _ViewController;
}

}
