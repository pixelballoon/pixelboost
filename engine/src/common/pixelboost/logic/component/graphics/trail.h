#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{

    class BufferRenderable;
    class IndexBuffer;
    class MaterialResource;
    class Message;
    class VertexBuffer;
    
    class TrailComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        TrailComponent(Entity* parent);
        virtual ~TrailComponent();

    public:
        void SetMinDistance(float minDistance);
        
        void SetMaterial(const std::string& file, const std::string& pool = "default");
        std::shared_ptr<MaterialResource> GetMaterial();
        
        BufferRenderable* GetRenderable();
        
    private:
        void OnTransformChanged(const pb::Message& message);
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        BufferRenderable* _Renderable;
        std::shared_ptr<MaterialResource> _Material;
        
        int _MaxElements;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        float _MinDistance;
        float _Length;
        std::vector<glm::vec2> _Points;
    };
    
}
