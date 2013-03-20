#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(glm::vec3 min, glm::vec3 max);
        
        bool IsValid();
        void Invalidate();
        
        glm::vec3 GetCenter();
        glm::vec3 GetSize();
        
        void Set(glm::vec3 min, glm::vec3 max);
        
        void Expand(glm::vec3 point);
        void Expand(const BoundingBox& box);
        
        bool Contains(glm::vec3 point);
        bool Intersects(const BoundingBox& box);
        
    private:
        glm::vec3 _Min;
        glm::vec3 _Max;
        bool _Valid;
    };
}
