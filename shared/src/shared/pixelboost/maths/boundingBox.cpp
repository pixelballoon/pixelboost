#include "pixelboost/maths/boundingBox.h"

using namespace pb;

BoundingBox::BoundingBox()
    : _Min(glm::vec3(0,0,0))
    , _Max(glm::vec3(0,0,0))
    , _Valid(false)
{
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max)
{
    Set(min, max);
}

bool BoundingBox::IsValid()
{
    return _Valid;
}

void BoundingBox::Invalidate()
{
    _Valid = false;
}

glm::vec3 BoundingBox::GetCenter()
{
    return (_Min+_Max)/2.f;
}

glm::vec3 BoundingBox::GetSize()
{
    return _Max-_Min;
}

void BoundingBox::Set(glm::vec3 min, glm::vec3 max)
{
    _Min = min;
    _Max = max;
    _Valid = true;
}

void BoundingBox::Expand(glm::vec3 point)
{
    if (!_Valid)
    {
        _Min = _Max = point;
        _Valid = true;
        return;
    }
    
    if (point.x < _Min.x)
        _Min.x = point.x;
    else if (point.x > _Max.x)
        _Max.x = point.x;
        
    if (point.y < _Min.y)
        _Min.y = point.y;
    else if (point.y > _Max.y)
        _Max.y = point.y;

    if (point.z < _Min.z)
        _Min.z = point.z;
    else if (point.z > _Max.z)
        _Max.z = point.z;
}

void BoundingBox::Expand(const BoundingBox& box)
{
    if (!box._Valid)
        return;
    
    if (!_Valid)
    {
        _Min = box._Min;
        _Max = box._Max;
        _Valid = true;
        return;
    }
    
    if (box._Min.x < _Min.x)
        _Min.x = box._Min.x;
    
    if (box._Max.x > _Max.x)
        _Max.x = box._Max.x;

    if (box._Min.y < _Min.y)
        _Min.y = box._Min.y;

    if (box._Max.y > _Max.y)
        _Max.y = box._Max.y;
    
    if (box._Min.z < _Min.z)
        _Min.z = box._Min.z;
    
    if (box._Max.z > _Max.z)
        _Max.z = box._Max.z;
}

bool BoundingBox::Contains(glm::vec3 point)
{
    return (point.x >= _Min.x && point.x <= _Max.x && point.y >= _Min.y && point.y <= _Max.y && point.z >= _Min.z && point.z <= _Max.z);
}

bool BoundingBox::Intersects(const BoundingBox& box)
{
    return !(box._Min.x > _Max.x || box._Max.x < _Min.x || box._Min.y > _Max.y || box._Max.y < _Min.y || box._Min.z > _Max.z || box._Max.z < _Min.z);
}
