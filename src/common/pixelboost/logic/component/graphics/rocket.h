#pragma once

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/logic/component.h"

namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}

namespace pb
{
 
    class Message;
    class RocketKeyboardHandler;
    class RocketMouseHandler;
    class RocketRenderable;
        
    class RocketComponent : public Component
    {
    public:
        RocketComponent(Entity* entity, glm::vec2 size);
        ~RocketComponent();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        void SetLayer(int layer);
        int GetLayer();
        
        Rocket::Core::Context* GetContext();
        
        void LoadRML(FileLocation location, const std::string& filename);
        
    private:
        void OnUpdate(const pb::Message& message);
        
        Rocket::Core::Context* _Context;
        RocketRenderable* _Renderable;
        
        RocketKeyboardHandler* _KeyboardHandler;
        RocketMouseHandler* _MouseHandler;
    };
    
}
