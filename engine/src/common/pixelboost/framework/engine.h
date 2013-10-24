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
class TextRenderer;
class GameCenter;
class GraphicsDevice;
class JoystickManager;
class KeyboardManager;
class ModelRenderer;
class MouseManager;
class NetworkDiscoveryServer;
class NetworkServer;
class ParticleRenderer;
class PrimitiveRenderer;
class Renderer;
class Screen;
class SpriteRenderer;
class TouchManager;
    
class Engine
{
protected:
	Engine(void* platformContext, int argc, const char** argv, bool enableNetworkDebug=true);
    
public:
    virtual ~Engine();
    
    static void RegisterLuaClass(lua_State* state);
    
    static Engine* Create(void* platformContext, int argc, const char** argv);
	
	static Engine* Instance();
	
	virtual void Initialise();

    void Quit() const;
    
    GameCenter* GetGameCenter() const;
    NetworkServer* GetDebugNetworkServer() const;
    
    JoystickManager* GetJoystickManager() const;
    KeyboardManager* GetKeyboardManager() const;
    MouseManager* GetMouseManager() const;
	TouchManager* GetTouchManager() const;
    
    float GetGameTime();
    float GetTotalTime();
    
    virtual bool IsLandscape();
    virtual bool AllowFrameskip();
    
    virtual void OnMemoryWarning();

    virtual void OnAndroidBackButton();
    virtual void OnAndroidMenuButton();

    virtual void OnAppLoseFocus();
    virtual void OnAppGainFocus();
    virtual void OnAppQuit();

	virtual void Update(float timeDelta, float gameDelta);
	void Render();
    
    void* GetPlatformContext();
	
private:
	static Engine* _Instance;
    
    int _ArgC;
    const char** _ArgV;    
    
    FileSystem* _FileSystem;
    
    GameCenter* _GameCenter;

    JoystickManager* _JoystickManager;
    KeyboardManager* _KeyboardManager;
    MouseManager* _MouseManager;
	TouchManager* _TouchManager;

    bool _NetworkDebugEnabled;
    NetworkDiscoveryServer* _DebugDiscovery;
    NetworkServer* _DebugNetwork;
    DebugDatabaseHandler* _DebugDatabaseHandler;
	
	float _GameTime;
    float _TotalTime;

    void* _PlatformContext;
};
    
}
