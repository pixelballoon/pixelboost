#pragma once

#include "glm/glm.hpp"

namespace pb
{
    class Ray
    {
    public:
        Ray();
        Ray(glm::vec3 origin, glm::vec3 direction);
        
    public:
        glm::vec3 Origin;
        glm::vec3 Direction;
    };
}
