#include "optionparser/optionparser.h"

#include "pixelboost/audio/audioManagerSimple.h"
#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/framework/screen.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/renderer/text/textRenderer.h"
#include "pixelboost/input/joystickManager.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/network/gameCenter.h"
#include "pixelboost/network/networkServer.h"
#include "pixelboost/resource/resourceManager.h"
#include "pixelboost/scripting/lua.h"

#include "pixelboost/debug/log.h"

using namespace pb;

Engine* Engine::_Instance = 0;

Engine::Engine(void* platformContext, int argc, const char** argv, bool enableNetworkDebug)
    : _GameTime(0)
    , _PlatformContext(platformContext)
    , _TotalTime(0)
    , _DebugNetwork(0)
    , _ArgC(argc)
    , _ArgV(argv)
{
    PbAssert(!_Instance);
    
    _NetworkDebugEnabled = enableNetworkDebug;
    _Instance = this;
}

Engine::~Engine()
{
    _Instance = 0;
    
#if !defined(PIXELBOOST_DISABLE_NETWORKING)
    delete NetworkManager::Instance();
#endif
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    delete _GameCenter;
#endif
    
    delete GuiRenderer::Instance();
    delete BufferRenderer::Instance();
    delete TextRenderer::Instance();
    delete ModelRenderer::Instance();
    delete ParticleRenderer::Instance();
    delete PrimitiveRenderer::Instance();
	delete SpriteRenderer::Instance();
    delete Renderer::Instance();

    delete _JoystickManager;
    delete _KeyboardManager;
    delete _MouseManager;
    delete _TouchManager;
    
    delete _FileSystem;
}

void Engine::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .beginClass<Engine>("Engine")
        .endClass()
    .endNamespace();
}

Engine* Engine::Instance()
{
    return _Instance;
}

void Engine::Initialise()
{
    LogSystem::Instance()->AddSubscriber(new LogSubscriberConsole());
    
    _FileSystem = new FileSystem(_ArgC > 0 ? _ArgV[0] : "");
    
    LogSystem::Instance()->AddSubscriber(new LogSubscriberFile("/pb.log"));
    
    if (_ArgC)
    {
        enum  optionIndex { kOptionUnknown, kOptionProject, kOptionCount };
        
        const option::Descriptor usage[] =
        {
            {kOptionUnknown,      0, "", "",         option::Arg::None,     ""},
            {kOptionProject,      0, "", "project",  option::Arg::Required, "  --project [location]" },
            {0,0,0,0,0,0}
        };
        option::Option options[kOptionCount], buffer[kOptionCount];
        option::Parser parse(usage, _ArgC-1, &_ArgV[1], options, buffer);
        
        if (options[kOptionProject])
        {
            _FileSystem->MountReadLocation(std::string(options[kOptionProject].arg) + "data/", "/", true);
        }
    }
    
    ResourceManager::Instance()->CreatePool("default");
    
    new Renderer();
    new BufferRenderer();
    new ModelRenderer();
    new ParticleRenderer();
    new SpriteRenderer();
    new PrimitiveRenderer();
    new TextRenderer();
    new GuiRenderer();
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    _GameCenter = new GameCenter();
#endif
    
    _JoystickManager = new JoystickManager();
    _KeyboardManager = new KeyboardManager();
    _MouseManager = new MouseManager();
    _TouchManager = new TouchManager();
    
#if !defined(PIXELBOOST_DISABLE_NETWORKING)
    new NetworkManager();
#endif
    
#if !defined(PIXELBOOST_DISABLE_NETWORKING) && !defined(PIXELBOOST_DISABLE_DEBUG)
    if (_NetworkDebugEnabled)
    {
        _DebugDiscovery = NetworkManager::Instance()->StartDiscoveryServer(9091);
        _DebugDiscovery->AddService("pb::debugvariable");
        _DebugNetwork = NetworkManager::Instance()->StartServer(9090, 8);
        _DebugDatabaseHandler = new DebugDatabaseHandler();
        _DebugNetwork->RegisterHandler(_DebugDatabaseHandler);
        _DebugNetwork->RegisterHandler(DebugVariableManager::Instance());
    }
#endif
}

GameCenter* Engine::GetGameCenter() const
{
    return _GameCenter;
}

NetworkServer* Engine::GetDebugNetworkServer() const
{
    return _DebugNetwork;
}

JoystickManager* Engine::GetJoystickManager() const
{
    return _JoystickManager;
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
    return true;
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

void Engine::Update(float timeDelta, float gameDelta)
{
    _GameTime += gameDelta;
    _TotalTime += timeDelta;
    
#ifndef PIXELBOOST_DISABLE_SOUND
    AudioManagerSimple::Instance()->Update(timeDelta);
#endif
    
    ResourceManager::Instance()->Update(timeDelta);

#if !defined(PIXELBOOST_DISABLE_NETWORKING)
    if (NetworkManager::Instance())
    {
        NetworkManager::Instance()->Update();
    }
#endif
}

void Engine::Render()
{
    Renderer::Instance()->Render();
}

void* Engine::GetPlatformContext()
{
    return _PlatformContext;
}
