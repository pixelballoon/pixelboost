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
        void SetMaxSegments(int maxSegments);
        int GetMaxSegments();
        
        void SetMinDistance(float minDistance);
        float GetMinDistance();
        
        void SetWidth(float width);
        float GetWidth();
        
        void SetLengthScale(float lengthScale);
        float GetLengthScale();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void SetMaterial(const std::string& file);
        std::shared_ptr<MaterialResource> GetMaterial();
        
        BufferRenderable* GetRenderable();
        
    private:
        void OnSetColor(const Message& message);
        void OnTransformChanged(const pb::Message& message);
        
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        BufferRenderable* _Renderable;
        std::shared_ptr<MaterialResource> _Material;
        
        int _MaxSegments;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        float _MinDistance;
        float _Width;
        float _LengthScale;
        float _Length;
        std::vector<glm::vec2> _Points;
        
        glm::vec4 _Tint;
    };
    
}
