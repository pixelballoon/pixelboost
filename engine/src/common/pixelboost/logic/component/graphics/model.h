#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class ModelRenderable;
    class ModelResource;
    class Model;
    
    class ModelComponent : public RenderableComponent<ModelRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        ModelComponent(Entity* parent);
        virtual ~ModelComponent();
        
    public:
        void SetShader(Shader* shader);
        void SetLayer(int layer);
        
        void SetModel(const std::string& filename);
        Model* GetModel();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
    private:
        void OnResourceLoaded(Resource* resource, bool loaded);
        void OnResourceUnloading(Resource* resource);

        std::shared_ptr<ModelResource> _Model;
    };
    
}
