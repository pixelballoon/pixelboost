#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(glm::vec3 min, glm::vec3 max);
        
        glm::vec3 GetCenter();
        glm::vec3 GetSize();
        
        void Invalidate();
        
        void Expand(glm::vec3 point);
        void Expand(const BoundingBox& box);
        
    private:
        glm::vec3 _Min;
        glm::vec3 _Max;
        bool _Valid;
    };
}
