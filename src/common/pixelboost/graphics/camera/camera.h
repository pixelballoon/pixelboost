#pragma once

#include "glm/glm.hpp"

#include "pixelboost/math/maths.h"

namespace pb
{

class Camera
{
public:
	Camera();
    virtual ~Camera();
    
	virtual void ApplyTransform() = 0;
};
    
class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Vec2 position = Vec2(0,0), Vec2 scale = Vec2(1,1));
    virtual void ApplyTransform();
    
    virtual glm::vec2 ConvertScreenToWorld(glm::vec2 screen);

    Vec2 Position;
    Vec2 Scale;
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
