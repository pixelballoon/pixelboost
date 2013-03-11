#include "optionparser/optionparser.h"

#include "pixelboost/audio/audioManagerSimple.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/framework/screen.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
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

namespace pb
{
    void RegisterResources();
}

Engine* Engine::_Instance = 0;

Engine::Engine(void* platformContext, int argc, const char** argv, bool enableNetworkDebug)
    : _GameTime(0)
    , _PlatformContext(platformContext)
    , _TotalTime(0)
    , _DebugNetwork(0)
{
    _Instance = this;
    
    LogSystem::Instance()->AddSubscriber(new LogSubscriberConsole());
    
    _FileSystem = new FileSystem(argc > 0 ? argv[0] : "");
    
    LogSystem::Instance()->AddSubscriber(new LogSubscriberFile("/pb.log"));
    
    if (argc)
    {
        enum  optionIndex { kOptionUnknown, kOptionProject, kOptionCount };
        
        const option::Descriptor usage[] =
        {
            {kOptionUnknown,      0, "", "",         option::Arg::None,     ""},
            {kOptionProject,      0, "", "project",  option::Arg::Required, "  --project [location]" },
            {0,0,0,0,0,0}
        };
        option::Option options[kOptionCount], buffer[kOptionCount];
        option::Parser parse(usage, argc-1, &argv[1], options, buffer);
        
        if (options[kOptionProject])
        {
            _FileSystem->MountReadLocation(std::string(options[kOptionProject].arg) + "data/", "/", true);
        }
    }
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer = new Renderer();
    _BufferRenderer = new BufferRenderer();
    _ModelRenderer  = new ModelRenderer();
    _ParticleRenderer = new ParticleRenderer();
    _SpriteRenderer = new SpriteRenderer();
    _PrimitiveRenderer = new PrimitiveRenderer();
    _FontRenderer = new FontRenderer();
    new GuiRenderer();
#endif

#ifndef PIXELBOOST_DISABLE_GAMECENTER
    _GameCenter = new GameCenter();
    _GameCenter->Connect();
#endif

#ifndef PIXELBOOST_DISABLE_INPUT
    _JoystickManager = new JoystickManager();
    _KeyboardManager = new KeyboardManager();
    _MouseManager = new MouseManager();
    _TouchManager = new TouchManager();
#endif
    
    NetworkManager* networkManager = new NetworkManager();

#ifndef PIXELBOOST_DISABLE_DEBUG
    if (enableNetworkDebug)
    {
        _DebugDiscovery = networkManager->StartDiscoveryServer(9091);
        _DebugDiscovery->AddService("pb::debugvariable");
        _DebugNetwork = networkManager->StartServer(9090, 8);
        _DebugDatabaseHandler = new DebugDatabaseHandler();
        _DebugNetwork->RegisterHandler(_DebugDatabaseHandler);
        _DebugNetwork->RegisterHandler(DebugVariableManager::Instance());
    }
#endif
}

Engine::~Engine()
{
    _Instance = 0;
    
    delete NetworkManager::Instance();
    
#ifndef PIXELBOOST_DISABLE_GAMECENTER
    delete _GameCenter;
#endif
    
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    delete GuiRenderer::Instance();
    delete _BufferRenderer;
    delete _FontRenderer;
    delete _ModelRenderer;
    delete _ParticleRenderer;
    delete _PrimitiveRenderer;
	delete _SpriteRenderer;
    delete _Renderer;
#endif

#ifndef PIXELBOOST_DISABLE_INPUT
    delete _JoystickManager;
    delete _KeyboardManager;
    delete _MouseManager;
    delete _TouchManager;
#endif
    
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
    RegisterResources();
}

GameCenter* Engine::GetGameCenter() const
{
    return _GameCenter;
}

NetworkServer* Engine::GetDebugNetworkServer() const
{
    return _DebugNetwork;
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

void Engine::Update(float timeDelta, float gameDelta)
{
    _GameTime += gameDelta;
    _TotalTime += timeDelta;
    
#ifndef PIXELBOOST_DISABLE_SOUND
    AudioManagerSimple::Instance()->Update(timeDelta);
#endif
    
    ResourceManager::Instance()->Update(timeDelta);
    
    NetworkManager::Instance()->Update();
}

void Engine::Render()
{
#ifndef PIXELBOOST_DISABLE_GRAPHICS
    _Renderer->Render();
#endif
}

void* Engine::GetPlatformContext()
{
    return _PlatformContext;
}
