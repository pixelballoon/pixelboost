#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class BoundingSphere
    {
    public:
        BoundingSphere();
        BoundingSphere(glm::vec3 position, float size);
        
        bool IsValid() const;
        void Invalidate();
        
        glm::vec3 GetCenter() const;
        float GetSize() const;
        
        void Set(glm::vec3 position, float size);
        
        void Expand(glm::vec3 point);
        void Expand(const BoundingSphere& sphere);
        
        bool Contains(glm::vec3 point) const;
        bool Intersects(const BoundingSphere& sphere) const;
        
    private:
        glm::vec3 _Center;
        float _Size;
        bool _Valid;
    };
}
