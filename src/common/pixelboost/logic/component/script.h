#pragma once

#include "pixelboost/logic/component.h"

struct lua_State;

namespace pb
{
    
    class LuaScript;
    class Message;

    class ScriptComponent : public Component
    {
    public:
        ScriptComponent(pb::Entity* parent);
        virtual ~ScriptComponent();
        
        static void RegisterLuaClass(lua_State* state);
        
        Uid GetType();
        static Uid GetStaticType();
        
        void SetSourceFile(const std::string& file);
        void SetSourceString(const std::string& source);
        
    private:
        void OnMessage(const pb::Message& message);
        void OnUpdate(const pb::Message& message);
        
        void LuaSendMessage(Uid target, const Message* message);
        void LuaWaitMessage(const std::string& message, lua_State* state);
        void LuaWaitSeconds(float seconds, lua_State* state);

        LuaScript* _Script;
        
        Uid _WaitMessage;
        float _WaitTime;
    };

}
