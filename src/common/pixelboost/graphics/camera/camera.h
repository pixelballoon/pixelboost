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
    
    Vec3 Position;
    float FieldOfView;
    float ZNear;
    float ZFar;
    
private:
    void BuildMatrix(float* m, float fov, float znear, float zfar);
};

}
