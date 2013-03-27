#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

Camera::Camera(glm::vec3 position, glm::vec3 rotation)
    : Position(position)
    , Rotation(rotation)
    , ZNear(1)
    , ZFar(1000)
{
    
}

Camera::~Camera()
{
    
}

void Camera::CalculateTransform(Viewport* viewport)
{
    Frustum.Set(ProjectionMatrix * ViewMatrix);
}

OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 rotation, glm::vec2 scale)
    : Camera(position, rotation)
    , Scale(scale)
{
    
}

Camera::CameraType OrthographicCamera::GetType() const
{
    return kCameraOrthographic;
}

void OrthographicCamera::CalculateTransform(Viewport* viewport)
{
    glm::vec2 viewportSize = viewport->GetSize() / 2.f;
    
    ProjectionMatrix = glm::ortho(-viewportSize.x/Scale.x, viewportSize.x/Scale.x, -viewportSize.y/Scale.y, viewportSize.y/Scale.y, ZNear, ZFar);
    ViewMatrix = CreateRotateMatrix(kRotationOrder_XYZ, Rotation);
    ViewMatrix = glm::translate(ViewMatrix, -Position);
    
    Camera::CalculateTransform(viewport);
}

glm::vec2 OrthographicCamera::ConvertScreenToWorld(glm::vec2 screen)
{
    glm::vec2 position = screen;
    
    position[0] = position[0] - GraphicsDevice::Instance()->GetDisplayResolution()[0]/2;
    position[1] = GraphicsDevice::Instance()->GetDisplayResolution()[1]/2 - position[1];
    
	position[0] /= GraphicsDevice::Instance()->GetDisplayDensity();
	position[1] /= GraphicsDevice::Instance()->GetDisplayDensity();
    
    position /= glm::vec2(Scale[0], Scale[1]);
    position += glm::vec2(Position[0], Position[1]);
	
	return position;
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, float fieldOfView)
    : Camera(position, rotation)
    , FieldOfView(fieldOfView)
{
    
}

Camera::CameraType PerspectiveCamera::GetType() const
{
    return kCameraPerspective;
}

void PerspectiveCamera::CalculateTransform(Viewport* viewport)
{
    glm::vec2 viewportSize = viewport->GetSize() / 2.f;
    
    ProjectionMatrix = glm::perspectiveFov(FieldOfView, viewportSize.x, viewportSize.y, ZNear, ZFar);
    ViewMatrix = glm::rotate(glm::mat4x4(), Rotation.x, glm::vec3(1,0,0));
    ViewMatrix = glm::rotate(ViewMatrix, Rotation.y, glm::vec3(0,1,0));
    ViewMatrix = glm::rotate(ViewMatrix, Rotation.z, glm::vec3(0,0,1));
    ViewMatrix = glm::translate(ViewMatrix, -Position);
    
    Camera::CalculateTransform(viewport);
}

OrbitalCamera::OrbitalCamera(glm::vec3 position, glm::vec3 rotation, float distance, float fieldOfView)
    : Camera(position, rotation)
    , Distance(distance)
    , FieldOfView(fieldOfView)
{
    
}

Camera::CameraType OrbitalCamera::GetType() const
{
    return kCameraOrbital;
}

void OrbitalCamera::CalculateTransform(Viewport* viewport)
{
    glm::vec2 viewportSize = viewport->GetSize() / 2.f;
    
    ProjectionMatrix = glm::perspectiveFov(FieldOfView, viewportSize.x, viewportSize.y, ZNear, ZFar);
    ViewMatrix = glm::translate(glm::mat4x4(), glm::vec3(0,0,-Distance));
    ViewMatrix = glm::rotate(ViewMatrix, Rotation.x, glm::vec3(1,0,0));
    ViewMatrix = glm::rotate(ViewMatrix, Rotation.y, glm::vec3(0,1,0));
    ViewMatrix = glm::rotate(ViewMatrix, Rotation.z, glm::vec3(0,0,1));
    ViewMatrix = glm::translate(ViewMatrix, -Position);
    
    Camera::CalculateTransform(viewport);
}
