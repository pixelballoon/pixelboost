#include "pixelboost/maths/boundingSphere.h"

using namespace pb;

BoundingSphere::BoundingSphere()
    : _Center(glm::vec3(0,0,0))
    , _Size(0)
    , _Valid(false)
{
}

BoundingSphere::BoundingSphere(glm::vec3 center, float size)
{
    Set(center, size);
}

bool BoundingSphere::IsValid()
{
    return _Valid;
}

void BoundingSphere::Invalidate()
{
    _Valid = false;
}

glm::vec3 BoundingSphere::GetCenter() const
{
    return _Center;
}

float BoundingSphere::GetSize() const
{
    return _Size;
}

void BoundingSphere::Set(glm::vec3 center, float size)
{
    _Center = center;
    _Size = size;
    _Valid = true;
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
    
    _Size = glm::max(_Size, glm::distance(_Center, point));
}

bool BoundingSphere::Contains(glm::vec3 point) const
{
    return glm::distance(point, _Center) < _Size;
}

bool BoundingSphere::Intersects(const BoundingSphere& box) const
{
    return glm::distance(_Center, box._Center) < _Size + box._Size;
}
