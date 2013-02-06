#include "pixelboost/logic/message.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

Message::Message(Entity* entity, Component* component)
    : _Entity(entity)
    , _Component(component)
{
    
}

Message::~Message()
{
    
}

void Message::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .beginClass<Message>("Message")
            .addFunction("GetType", &Message::GetType)
        .endClass()
    .endNamespace();
}

Entity* Message::GetEntity() const
{
    return _Entity;
}

Component* Message::GetComponent() const
{
    return _Component;
}
