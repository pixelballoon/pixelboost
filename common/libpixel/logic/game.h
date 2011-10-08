#ifndef LIBPIXEL__COMMON__LOGIC__GAME__H
#define LIBPIXEL__COMMON__LOGIC__GAME__H

#include <vector>

namespace libpixel
{

#ifndef LIBPIXEL_DISABLE_DEBUG
class DebugNetworkManager;
#endif

class FontRenderer;
class GameCenter;
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

#ifndef LIBPIXEL_DISABLE_DEBUG
    DebugNetworkManager* GetDebugNetworkManager() const;
#endif

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
    
    virtual bool UseHdGraphics();
    virtual bool IsLandscape();
	
	virtual void Update(float time);
	virtual void Render();
    
    void* GetViewController();
	
private:
	static Game* _Instance;
	
#ifndef LIBPIXEL_DISABLE_DEBUG
    DebugNetworkManager* _DebugNetworkManager;
#endif

    FontRenderer* _FontRenderer;
    GameCenter* _GameCenter;
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

#endif
