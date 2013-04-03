#include "pixelboost/maths/plane.h"
#include "pixelboost/maths/ray.h"

using namespace pb;

Plane::Plane()
{
    
}

Plane::Plane(glm::vec3 point, glm::vec3 normal)
{
    Point = point;
    Normal = normal;
}

std::pair<bool, glm::vec3> Plane::GetIntersection(const Ray& ray)
{
    float t = glm::dot(Normal, (Point - ray.Origin))/glm::dot(Normal, ray.Direction);
    
    if (t < 0)
        return std::make_pair(false, glm::vec3(0,0,0));
    
    return std::make_pair(true, ray.Origin + (ray.Direction * t));
}
