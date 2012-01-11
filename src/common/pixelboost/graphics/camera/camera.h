#pragma once

#include "pixelboost/math/maths.h"

namespace pixelboost
{

class Camera
{
public:
	Camera();
	virtual void ApplyTransform() = 0;
};
    
class OrthographicCamera : public Camera
{
public:
    OrthographicCamera();
    virtual void ApplyTransform();

    Vec2 Position;
};
    
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera();
    virtual void ApplyTransform();
    
    float FieldOfView;
    Vec2 Offset;
    Vec3 Position;
    float ZNear;
    float ZFar;
};

}
