#pragma once

#include <vector>

namespace pixelboost
{

class CustomRenderer;
class FontRenderer;
class GameCenter;
class GraphicsDevice;
class ModelRenderer;
class ParticleRenderer;
class PrimitiveRenderer;
class Renderer;
class ResourceManager;
class Screen;
class SoundManager;
class SpriteRenderer;
class TouchManager;
    
class Game
{
public:
	Game(void* viewController);
	virtual ~Game();
	
	static Game* Instance();
	
	virtual void Initialise();

    CustomRenderer* GetCustomRenderer() const;
    FontRenderer* GetFontRenderer() const;
    GameCenter* GetGameCenter() const;
    ModelRenderer* GetModelRenderer() const;
	ResourceManager* GetResourceManager() const;
    ParticleRenderer* GetParticleRenderer() const;
    PrimitiveRenderer* GetPrimitiveRenderer() const;
    Renderer* GetRenderer() const;
	SpriteRenderer* GetSpriteRenderer() const;
	TouchManager* GetTouchManager() const;
    
    float GetGameTime();
    float GetTotalTime();
    
    virtual bool IsLandscape();
	
	virtual void Update(float time);
	virtual void Render();
    
    void* GetViewController();
	
private:
	static Game* _Instance;

    CustomRenderer* _CustomRenderer;
    FontRenderer* _FontRenderer;
    GameCenter* _GameCenter;
    ModelRenderer* _ModelRenderer;
    ParticleRenderer* _ParticleRenderer;
    PrimitiveRenderer* _PrimitiveRenderer;
    Renderer* _Renderer;
	ResourceManager* _ResourceManager;
	SoundManager* _SoundManager;
	SpriteRenderer* _SpriteRenderer;
	TouchManager* _TouchManager;
	
	float _GameTime;
    float _TotalTime;

    void* _ViewController;
};
    
}