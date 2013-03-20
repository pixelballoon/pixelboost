#include "pixelboost/maths/boundingFrustum.h"
#include "pixelboost/maths/boundingSphere.h"

using namespace pb;

BoundingFrustum::BoundingFrustum()
    : _Valid(false)
{
    
}

BoundingFrustum::BoundingFrustum(const glm::mat4x4& viewProjectionMatrix)
{
    Set(viewProjectionMatrix);
}

BoundingFrustum::BoundingFrustum(glm::vec4 left, glm::vec4 right, glm::vec4 top, glm::vec4 bottom, glm::vec4 front, glm::vec4 back)
{
    Set(left, right, top, bottom, front, back);
}

bool BoundingFrustum::IsValid() const
{
    return _Valid;
}

void BoundingFrustum::Invalidate()
{
    _Valid = false;
}

void BoundingFrustum::Set(const glm::mat4x4& viewProjectionMatrix)
{
    glm::vec4 left, right, top, bottom, front, back;
    
    left.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][0];
    left.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][0];
    left.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][0];
    left.w = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][0];
    
    right.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][0];
    right.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][0];
    right.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][0];
    right.w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][0];
    
    top.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][1];
    top.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][1];
    top.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][1];
    top.w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][1];
    
    bottom.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][1];
    bottom.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][1];
    bottom.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][1];
    bottom.w = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][1];
    
    front.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][2];
    front.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][2];
    front.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][2];
    front.w = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][2];
    
    back.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][2];
    back.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][2];
    back.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][2];
    back.w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][2];
    
    Set(left, right, top, bottom, front, back);
}

void BoundingFrustum::Set(glm::vec4 left, glm::vec4 right, glm::vec4 top, glm::vec4 bottom, glm::vec4 front, glm::vec4 back)
{
    _Planes[0] = left;
    _Planes[1] = right;
    _Planes[2] = top;
    _Planes[3] = bottom;
    _Planes[4] = front;
    _Planes[5] = back;
    
    Normalise();
    
    _Valid = true;
}

bool BoundingFrustum::Contains(glm::vec3 point) const
{
    return false;
}

bool BoundingFrustum::Intersects(const BoundingSphere& sphere) const
{
    for (int i=0; i<6; i++)
    {
        if (glm::dot(glm::vec4(sphere.GetCenter(), 1), _Planes[i]) <= -sphere.GetSize())
            return false;
    }
    
    return true;
}

void BoundingFrustum::Normalise()
{
    for (int i=0; i<6; i++)
    {
        float length = glm::length(glm::vec3(_Planes[i].x, _Planes[i].y, _Planes[i].z));
        _Planes[i] = _Planes[i] / length;
    }
}
