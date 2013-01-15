#pragma once

#include "glm/glm.hpp"

#include "pixelboost/maths/boundingFrustum.h"

namespace pb
{
    
class Viewport;

class Camera
{
public:
    enum CameraType
    {
        kCameraOrthographic,
        kCameraPerspective,
    };
    
public:
	Camera(glm::vec3 position, glm::vec3 rotation);
    virtual ~Camera();
    
    virtual CameraType GetType() const = 0;
    
	virtual void CalculateTransform(Viewport* viewport) = 0;
    
public:
    BoundingFrustum Frustum;
    glm::mat4x4 ProjectionMatrix;
    glm::mat4x4 ViewMatrix;
    
    glm::vec3 Position;
    glm::vec3 Rotation;
    
    float ZNear;
    float ZFar;
};
    
class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(glm::vec3 position = glm::vec3(0,0,500), glm::vec3 rotation = glm::vec3(0,0,0), glm::vec2 scale = glm::vec2(1,1));
    
    virtual CameraType GetType() const;
    
    virtual void CalculateTransform(Viewport* viewport);
    
    virtual glm::vec2 ConvertScreenToWorld(glm::vec2 screen);
    
public:
    glm::vec2 Scale;
};
    
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(glm::vec3 position = glm::vec3(0,0,0), glm::vec3 rotation = glm::vec3(0,0,0));
    
    virtual CameraType GetType() const;
    
    virtual void CalculateTransform(Viewport* viewport);
    
public:
    float FieldOfView;
};

}
