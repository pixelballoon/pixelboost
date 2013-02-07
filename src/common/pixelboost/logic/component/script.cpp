#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/logic/component/script.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

ScriptComponent::ScriptComponent(pb::Entity* parent)
    : Component(parent)
{
    _Active = true;
    _Script = LuaManager::Instance()->CreateScript(this);
    
    AddThread(_Script->GetState());
    
    GetParent()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &ScriptComponent::OnUpdate));
}

ScriptComponent::~ScriptComponent()
{
    GetParent()->UnregisterMessageHandler(MessageHandler(this, &ScriptComponent::OnMessage));
    GetParent()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &ScriptComponent::OnUpdate));
    
    delete _Script;
}

void ScriptComponent::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .deriveClass<ScriptComponent, Component>("ScriptComponent")
            .addFunction("SendMessage", &ScriptComponent::LuaSendMessage)
            .addFunction("SendMessageTarget", &ScriptComponent::LuaSendMessageTarget)
            .addFunction("WaitMessage", &ScriptComponent::LuaWaitMessage)
            .addFunction("WaitSeconds", &ScriptComponent::LuaWaitSeconds)
        .endClass()
    .endNamespace();
}

Uid ScriptComponent::GetType()
{
    return GetStaticType();
}

Uid ScriptComponent::GetStaticType()
{
    return TypeHash("pb::ScriptComponent");
}

void ScriptComponent::SetActive(bool active)
{
    _Active = active;
    
    if (active == true)
    {
        GetParent()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &ScriptComponent::OnUpdate));
    }
}

void ScriptComponent::SetSourceFile(const std::string& filename)
{
    File* file = FileSystem::Instance()->OpenFile(filename);
    
    if (!file)
        return;
    
    std::string source;
    file->ReadAll(source);
    delete file;
    
    SetSourceString(source);
}

void ScriptComponent::SetSourceString(const std::string& source)
{
    _Script->Load(source);
}

void ScriptComponent::OnMessage(const Message& message)
{
    for (std::map<int, Uid>::iterator messageIt = _WaitMessage.begin(); messageIt != _WaitMessage.end(); ++messageIt)
    {
        if (message.GetType() == messageIt->second)
        {
            messageIt->second = 0;
        }
    }
}

void ScriptComponent::OnUpdate(const Message& message)
{
    if (!_Active)
        return;
    
    const UpdateMessage& updateMessage = static_cast<const UpdateMessage&>(message);
    
    int pendingMessages = _WaitMessage.size();
    
    for (std::map<lua_State*, int>::iterator it = _Threads.begin(); it != _Threads.end();)
    {
        if (!UpdateState(it->second, updateMessage.GetGameDelta()))
        {
            luaL_unref(_Script->GetState(), LUA_REGISTRYINDEX, it->second);
            _WaitMessage.erase(it->second);
            _WaitTime.erase(it->second);
            _Threads.erase(it++);
        } else {
            ++it;
        }
    }
    
    for (std::map<int, Uid>::iterator it = _WaitMessage.begin(); it != _WaitMessage.end();)
    {
        if (it->second == 0)
        {
            _WaitMessage.erase(it++);
        } else {
            ++it;
        }
    }
    
    for (std::map<int, float>::iterator it = _WaitTime.begin(); it != _WaitTime.end();)
    {
        if (it->second <= 0.f)
        {
            _WaitTime.erase(it++);
        } else {
            ++it;
        }
    }
  
    if (_WaitMessage.size() == 0 && pendingMessages > 0)
    {
        GetParent()->UnregisterMessageHandler(MessageHandler(this, &ScriptComponent::OnMessage));
    }
    
    if (_Threads.size() == 0)
    {
        _Active = false;
        GetParent()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &ScriptComponent::OnUpdate));
    }
}

bool ScriptComponent::UpdateState(int threadIndex, float gameDelta)
{
    std::map<int, Uid>::iterator messageIt = _WaitMessage.find(threadIndex);
    if (messageIt != _WaitMessage.end())
    {
        if (messageIt->second != 0)
        {
            return true;
        }
    }
    
    std::map<int, float>::iterator timeIt = _WaitTime.find(threadIndex);
    if (timeIt != _WaitTime.end())
    {
        timeIt->second -= gameDelta;
        
        if (timeIt->second > 0.f)
        {
            return true;
        }
    }
    
    lua_rawgeti(_Script->GetState(), LUA_REGISTRYINDEX, threadIndex);
    lua_State* state = lua_tothread(_Script->GetState(), -1);
    lua_pop(_Script->GetState(), 1);
    
    _ScriptYield = false;
    
    int status = lua_resume(state, 0, 0);
    
    switch (status)
    {
        case LUA_OK:
            break;
        case LUA_YIELD:
            break;
        case LUA_ERRRUN:
            if (lua_isstring(_Script->GetState(), -1))
            {
                PbLogError("lua.script", "Lua runtime error (%s)\n", lua_tostring(_Script->GetState(), -1));
            } else {
                PbLogError("lua.script", "Lua runtime error (Unknown)\n");
            }
            break;
        case LUA_ERRSYNTAX:
            PbLogError("lua.script", "Lua syntax error\n");
            break;
        case LUA_ERRMEM:
            PbLogError("lua.script", "Lua malloc error\n");
            break;
        case LUA_ERRGCMM:
            PbLogError("lua.script", "Lua GC error\n");
            break;
        case LUA_ERRERR:
            PbLogError("lua.script", "Lua error handler error\n");
            break;
    }
    
    if (status != LUA_YIELD)
    {
        return false;
    }
    
    return state == _Script->GetState() || _ScriptYield;
}

void ScriptComponent::LuaSendMessage(const Message* message)
{
    GetScene()->SendMessage(GetParentUid(), *message);
}

void ScriptComponent::LuaSendMessageTarget(Uid target, const Message* message)
{
    GetScene()->SendMessage(target, *message);
}

void ScriptComponent::LuaWaitMessage(const std::string& message, lua_State* state)
{
    GetParent()->RegisterMessageHandler(MessageHandler(this, &ScriptComponent::OnMessage));
    
    int threadIndex = AddThread(state);
    _WaitMessage[threadIndex] = pb::RuntimeTypeHash(message);
    
    _ScriptYield = true;
    
    lua_yield(state, 0);
}

void ScriptComponent::LuaWaitSeconds(float seconds, lua_State* state)
{
    int threadIndex = AddThread(state);
    _WaitTime[threadIndex] = seconds;
    
    _ScriptYield = true;
    
    lua_yield(state, 0);
}

int ScriptComponent::AddThread(lua_State* state)
{
    std::map<lua_State*, int>::iterator it = _Threads.find(state);
    
    if (it == _Threads.end())
    {
        lua_pushthread(state);
        int threadIndex = luaL_ref(state, LUA_REGISTRYINDEX);
        _Threads[state] = threadIndex;
        return threadIndex;
    }
    
    return it->second;
}
