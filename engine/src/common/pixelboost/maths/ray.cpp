#include "pixelboost/maths/ray.h"

using namespace pb;

Ray::Ray()
{
    
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
    Origin = origin;
    Direction = direction;
}
