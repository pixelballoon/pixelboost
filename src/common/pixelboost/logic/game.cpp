#include "pixelboost/audio/soundManager.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/render/common/renderer.h"
#include "pixelboost/graphics/render/custom/customRenderer.h"
#include "pixelboost/graphics/render/font/fontRenderer.h"
#include "pixelboost/graphics/render/model/modelRenderer.h"
#include "pixelboost/graphics/render/particle/particleRenderer.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/render/sprite/spriteRenderer.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/game.h"
#include "pixelboost/logic/screen.h"
#include "pixelboost/network/gameCenter.h"
#include "pixelboost/network/networkServer.h"

namespace pixelboost
{

Game* Game::_Instance = 0;

Game::Game(void* viewController)
    : _GameTime(0)
    , _TotalTime(0)
    , _ViewController(viewController)
{
    _Instance = this;

    _TouchManager = new TouchManager();
    
    _GameCenter = new GameCenter();
    
    _CustomRenderer = new CustomRenderer();
    _FontRenderer = new FontRenderer();
    _ModelRenderer  = new ModelRenderer();
    _ParticleRenderer = new ParticleRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    _SpriteRenderer = new SpriteRenderer();

    _Renderer = new Renderer();
    _Renderer->AddRenderer(_SpriteRenderer);
    _Renderer->AddRenderer(_ModelRenderer);
    _Renderer->AddRenderer(_CustomRenderer);
    _Renderer->AddRenderer(_ParticleRenderer);
    _Renderer->AddRenderer(_PrimitiveRenderer);
    _Renderer->AddRenderer(_FontRenderer);
    
    _SoundManager = new SoundManager();
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    _GameCenter->Connect();
#endif
}

Game::~Game()
{
    _Instance = 0;
    
    _Renderer->RemoveRenderer(_CustomRenderer);
    _Renderer->RemoveRenderer(_FontRenderer);
    _Renderer->RemoveRenderer(_ModelRenderer);
    _Renderer->RemoveRenderer(_ParticleRenderer);
    _Renderer->RemoveRenderer(_PrimitiveRenderer);
    _Renderer->RemoveRenderer(_SpriteRenderer);

    delete _CustomRenderer;
    delete _FontRenderer;
    delete _GameCenter;
    delete _ModelRenderer;
    delete _ParticleRenderer;
    delete _PrimitiveRenderer;
    delete _Renderer;
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
