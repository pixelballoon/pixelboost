#pragma once

#include <utility>

#include "glm/glm.hpp"

namespace pb
{
    class Ray;
    
    class Plane
    {
    public:
        Plane();
        Plane(glm::vec3 point, glm::vec3 normal);
        
        std::pair<bool, glm::vec3> GetIntersection(const Ray& ray);
        
    public:
        glm::vec3 Point;
        glm::vec3 Normal;
    };
}
