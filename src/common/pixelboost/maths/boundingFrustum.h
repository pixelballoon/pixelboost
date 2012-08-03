#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class BoundingSphere;
    
    class BoundingFrustum
    {
    public:
        BoundingFrustum();
        BoundingFrustum(const glm::mat4x4& viewProjectionMatrix);
        BoundingFrustum(glm::vec4 left, glm::vec4 right, glm::vec4 top, glm::vec4 bottom, glm::vec4 front, glm::vec4 back);
        
        bool IsValid();
        void Invalidate();
        
        void Set(const glm::mat4x4& viewProjectionMatrix);
        void Set(glm::vec4 left, glm::vec4 right, glm::vec4 top, glm::vec4 bottom, glm::vec4 front, glm::vec4 back);
        
        bool Contains(glm::vec3 point);
        bool Intersects(const BoundingSphere& sphere);
        
    private:
        void Normalise();
        
        glm::vec4 _Planes[6];
        
        bool _Valid;
    };
}
