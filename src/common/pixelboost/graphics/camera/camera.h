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
    OrthographicCamera(Vec2 position = Vec2(0,0), Vec2 scale = Vec2(1,1));
    virtual void ApplyTransform();

    Vec2 Position;
    Vec2 Scale;
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
