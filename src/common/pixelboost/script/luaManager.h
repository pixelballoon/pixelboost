#pragma once

#include <string>

struct lua_State;

namespace pixelboost
{
    class LuaManager
    {
    public:
        LuaManager();
        ~LuaManager();
        
        void Open(const std::string& filename);
        
    private:
        lua_State* _State;
    };
}
