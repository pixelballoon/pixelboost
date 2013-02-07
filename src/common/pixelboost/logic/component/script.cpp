#include "glm/gtc/matrix_transform.hpp"

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
    _WaitMessage = 0;
    _WaitTime = 0.f;
    
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
    if (_WaitMessage && message.GetType() == _WaitMessage)
    {
        GetParent()->UnregisterMessageHandler(MessageHandler(this, &ScriptComponent::OnMessage));
        
        _WaitMessage = 0;
        _WaitTime = 0;
    }
}

void ScriptComponent::OnUpdate(const Message& message)
{
    if (_WaitMessage || !_Active)
        return;
    
    const UpdateMessage& updateMessage = static_cast<const UpdateMessage&>(message);
    
    _WaitTime = glm::max(_WaitTime - updateMessage.GetGameDelta(), 0.f);
    
    if (_WaitTime <= 0.f)
    {
        _Script->Step();
    }
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
    
    _WaitMessage = pb::RuntimeTypeHash(message);
    lua_yield(state, 0);
}

void ScriptComponent::LuaWaitSeconds(float seconds, lua_State* state)
{
    _WaitTime += seconds;
    lua_yield(state, 0);
}
