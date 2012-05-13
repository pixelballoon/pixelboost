#include "pixelboost/db/entity.h"
#include "pixelboost/db/manager.h"

#include "pixelboost/external/lua/lua.hpp"

using namespace pb;
    
Entity::Entity(Uid uid, Uid type, void* data)
    : Struct(uid, type, data)
{
    lua_State* state = pb::DatabaseManager::Instance()->GetLuaState();
    
    lua_getfield(state, -1, "transform");

    lua_getfield(state, -1, "t");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Position = Vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);
    
    lua_getfield(state, -1, "r");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Rotation = Vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);
    
    lua_getfield(state, -1, "s");
    lua_rawgeti(state, -1, 3);
    lua_rawgeti(state, -2, 2);
    lua_rawgeti(state, -3, 1);
    _Scale = Vec3(lua_tonumber(state, -1), lua_tonumber(state, -2), lua_tonumber(state, -3));
    lua_pop(state, 4);

    lua_pop(state, 1);
}
    
Entity::~Entity()
{
    
}

const Vec3& Entity::GetPosition() const
{
    return _Position;
}

const Vec3& Entity::GetRotation() const
{
    return _Rotation;
}

const Vec3& Entity::GetScale() const
{
    return _Scale;
}

