#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

Component::Component(Entity* parent)
    : _Parent(parent)
    , _State(kComponentCreated)
{
    
}

Component::~Component()
{

}

void Component::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .beginClass<Component>("Component")
            .addFunction("GetEntity", &Component::GetEntity)
            .addFunction("GetEntityUid", &Component::GetEntityUid)
        .endClass()
    .endNamespace();
}

bool Component::IsA(Uid type) const
{
    return type == GetType();
}

Uid Component::GetType() const
{
    return GetStaticType();
}

Uid Component::GetStaticType()
{
    return TypeHash("pb::Component");
}

Scene* Component::GetScene()
{
    return _Parent->GetScene();
}

Entity* Component::GetEntity()
{
    return _Parent;
}

pb::Uid Component::GetEntityUid()
{
    return _Parent->GetUid();
}
