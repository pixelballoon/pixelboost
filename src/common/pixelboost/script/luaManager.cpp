#include "pixelboost/script/luaManager.h"
#include "pixelboost/external/lua/lua.hpp"

using namespace pixelboost;

LuaManager::LuaManager()
{
    _State = luaL_newstate();
    luaL_openlibs(_State);
}

LuaManager::~LuaManager()
{
    lua_close(_State);
}

void LuaManager::Open(const std::string& filename)
{
    if (luaL_loadfile(_State, filename.c_str()) || lua_pcall(_State, 0, 0, 0))
    {
        printf("Can't open file: %s", lua_tostring(_State, -1));
    }
}
