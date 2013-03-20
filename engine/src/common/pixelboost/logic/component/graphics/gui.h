#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/graphics/renderable.h"
#include "pixelboost/logic/message/graphics/gui.h"

namespace pb
{
    
    class GuiRenderable;
    class GuiSystem;
    class Message;
    class ShaderResource;
    
    struct GuiInputEvent;
    struct GuiState;
    
    class GuiComponent : public RenderableComponent<GuiRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        GuiComponent(Entity* parent);
        virtual ~GuiComponent();
        
    public:
        void SetSize(glm::vec2 size);
        glm::vec2 GetSize();
        
        void SetGeometryShader(const std::string& filename);
        void SetSpriteShader(const std::string& filename);
        void SetTextShader(const std::string& filename);
        
    private:
        void OnGui(GuiState& state, GuiSystem* system, GuiRenderMessage::EventType eventType);
        
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        glm::vec2 _Size;
        
        std::shared_ptr<ShaderResource> _GeometryShader;
        std::shared_ptr<ShaderResource> _SpriteShader;
        std::shared_ptr<ShaderResource> _TextShader;
        
        friend class GuiSystem;
    };
    
}
