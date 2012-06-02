#include "pixelboost/db/database.h"
#include "pixelboost/db/entity.h"

#include "pixelboost/external/lua/lua.hpp"

using namespace pb;
    
Entity::Entity(Uid uid, Uid type, void* data)
    : Struct(uid, type, data)
{

}
    
Entity::~Entity()
{
    
}

void Entity::Load()
{
    lua_State* state = pb::Database::Instance()->GetLuaState();
    
    lua_getfield(state, -1, "transform");
    
    lua_getfield(state, -1, "t");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Position = glm::vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);
    
    lua_getfield(state, -1, "r");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Rotation = glm::vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);
    
    lua_getfield(state, -1, "s");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Scale = glm::vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);
    
    lua_pop(state, 1);
}

const glm::vec3& Entity::GetPosition() const
{
    return _Position;
}

const glm::vec3& Entity::GetRotation() const
{
    return _Rotation;
}

const glm::vec3& Entity::GetScale() const
{
    return _Scale;
}
