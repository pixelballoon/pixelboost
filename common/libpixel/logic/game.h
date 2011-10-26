#pragma once

#include <vector>

namespace libpixel
{

class FontRenderer;
class GameCenter;
class GraphicsDevice;
class ModelRenderer;
class PrimitiveRenderer;
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

    FontRenderer* GetFontRenderer() const;
    GameCenter* GetGameCenter() const;
    ModelRenderer* GetModelRenderer() const;
	ResourceManager* GetResourceManager() const;
    PrimitiveRenderer* GetPrimitiveRenderer() const;
    SoundManager* GetSoundManager() const;
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

    FontRenderer* _FontRenderer;
    GameCenter* _GameCenter;
    GraphicsDevice* _GraphicsDevice;
    ModelRenderer* _ModelRenderer;
    PrimitiveRenderer* _PrimitiveRenderer;
	ResourceManager* _ResourceManager;
	SoundManager* _SoundManager;
	SpriteRenderer* _SpriteRenderer;
	TouchManager* _TouchManager;
	
	float _GameTime;
    float _TotalTime;

    void* _ViewController;
};
    
}
