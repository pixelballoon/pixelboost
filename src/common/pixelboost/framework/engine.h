#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

struct lua_State;

namespace pb
{

class BufferRenderer;
class DebugDatabaseHandler;
class FileSystem;
class FontRenderer;
class GameCenter;
class GraphicsDevice;
class JoystickManager;
class KeyboardManager;
class ModelRenderer;
class MouseManager;
class NetworkServer;
class ParticleRenderer;
class PrimitiveRenderer;
class Renderer;
class ResourceManager;
class Screen;
class SpriteRenderer;
class TouchManager;
    
class Engine
{
protected:
	Engine(void* platformContext, int argc, const char** argv);
    
public:
    virtual ~Engine();
    
    static void RegisterLuaClass(lua_State* state);
    
    static Engine* Create(void* platformContext, int argc, const char** argv);
	
	static Engine* Instance();
	
	virtual void Initialise();
    
    GameCenter* GetGameCenter() const;

    BufferRenderer* GetBufferRenderer() const;
    FontRenderer* GetFontRenderer() const;
    ModelRenderer* GetModelRenderer() const;
	ResourceManager* GetResourceManager() const;
    ParticleRenderer* GetParticleRenderer() const;
    PrimitiveRenderer* GetPrimitiveRenderer() const;
	SpriteRenderer* GetSpriteRenderer() const;
    
    JoystickManager* GetJoystickManager() const;
    KeyboardManager* GetKeyboardManager() const;
    MouseManager* GetMouseManager() const;
	TouchManager* GetTouchManager() const;
    
    float GetGameTime();
    float GetTotalTime();
    
    virtual bool IsLandscape();
    virtual bool AllowFrameskip();
    
    virtual void OnMemoryWarning();

    virtual void OnAppLoseFocus();
    virtual void OnAppGainFocus();
    virtual void OnAppQuit();
	
	virtual void Update(float timeDelta, float gameDelta);
	void Render();
    
    void* GetPlatformContext();
	
private:
	static Engine* _Instance;
    
    FileSystem* _FileSystem;
    
    GameCenter* _GameCenter;
    ResourceManager* _ResourceManager;

    BufferRenderer* _BufferRenderer;
    FontRenderer* _FontRenderer;
    ModelRenderer* _ModelRenderer;
    ParticleRenderer* _ParticleRenderer;
    PrimitiveRenderer* _PrimitiveRenderer;
    Renderer* _Renderer;
	SpriteRenderer* _SpriteRenderer;
    
    JoystickManager* _JoystickManager;
    KeyboardManager* _KeyboardManager;
    MouseManager* _MouseManager;
	TouchManager* _TouchManager;
    
    NetworkServer* _DebugNetwork;
    DebugDatabaseHandler* _DebugDatabaseHandler;
	
	float _GameTime;
    float _TotalTime;

    void* _PlatformContext;
};
    
}
