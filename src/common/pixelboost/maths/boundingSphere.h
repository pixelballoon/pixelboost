#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class BoundingSphere
    {
    public:
        BoundingSphere();
        BoundingSphere(glm::vec3 position, float size);
        
        bool IsValid();
        void Invalidate();
        
        glm::vec3 GetCenter();
        float GetSize();
        
        void Expand(glm::vec3 point);
        
        bool Contains(glm::vec3 point);
        bool Intersects(const BoundingSphere& sphere);
        
    private:
        glm::vec3 _Center;
        float _Size;
        bool _Valid;
    };
}
