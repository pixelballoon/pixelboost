#pragma once

#include "pixelboost/framework/definitions.h"

struct lua_State;

namespace pb
{

    class Component;
    class Entity;

    class Message
    {
    public:
        Message(Entity* entity, Component* component);
        virtual ~Message();
        
        static void RegisterLuaClass(lua_State* state);
        
        virtual Uid GetType() const = 0;
        
        Entity* GetEntity() const;
        Component* GetComponent() const;
        
    private:
        Entity* _Entity;
        Component* _Component;
    };
    
}
