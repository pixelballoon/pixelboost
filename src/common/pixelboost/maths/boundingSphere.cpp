#include "pixelboost/maths/boundingSphere.h"

using namespace pb;

BoundingSphere::BoundingSphere()
    : _Center(glm::vec3(0,0,0))
    , _Size(0)
    , _Valid(false)
{
}

BoundingSphere::BoundingSphere(glm::vec3 center, float size)
    : _Center(center)
    , _Size(size)
    , _Valid(true)
{
}

bool BoundingSphere::IsValid()
{
    return _Valid;
}

void BoundingSphere::Invalidate()
{
    _Valid = false;
}

glm::vec3 BoundingSphere::GetCenter()
{
    return _Center;
}

float BoundingSphere::GetSize()
{
    return _Size;
}

void BoundingSphere::Expand(glm::vec3 point)
{
    if (!_Valid)
    {
        _Center = point;
        _Size = 0.f;
        _Valid = true;
        return;
    }
    
    // TODO: Expand
}

bool BoundingSphere::Contains(glm::vec3 point)
{
    return glm::distance(point, _Center) < _Size;
}

bool BoundingSphere::Intersects(const BoundingSphere& box)
{
    return glm::distance(_Center, box._Center) < _Size + box._Size;
}
