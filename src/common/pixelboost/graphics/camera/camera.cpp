#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

using namespace pb;

Camera::Camera(glm::vec3 position, glm::vec3 rotation)
    : Position(position)
    , Rotation(rotation)
    , ZNear(0)
    , ZFar(100)
{
    
}

Camera::~Camera()
{
    
}

OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 rotation, glm::vec2 scale)
    : Camera(position, rotation)
    , Scale(scale)
{
    
}

void OrthographicCamera::CalculateTransform(Viewport* viewport)
{
    glm::vec2 viewportSize = viewport->GetSize() / 2.f;
    
    ProjectionMatrix = glm::ortho(-viewportSize.x/Scale.x, viewportSize.x/Scale.x, -viewportSize.y/Scale.y, viewportSize.y/Scale.y, ZNear, ZFar);
    ViewMatrix = glm::translate(glm::mat4x4(), -Position);
}

glm::vec2 OrthographicCamera::ConvertScreenToWorld(glm::vec2 screen)
{
    glm::vec2 position = screen;
    
    position[0] = position[0] - ScreenHelpers::GetScreenResolution()[0]/2;
    position[1] = ScreenHelpers::GetScreenResolution()[1]/2 - position[1];
    
	position[0] /= ScreenHelpers::GetDpu();
	position[1] /= ScreenHelpers::GetDpu();
    
    position /= glm::vec2(Scale[0], Scale[1]);
    position += glm::vec2(Position[0], Position[1]);
	
	return position;
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 rotation)
    : Camera(position, rotation)
    , FieldOfView(90.f)
{
}

void PerspectiveCamera::CalculateTransform(Viewport* viewport)
{
    glm::vec2 viewportSize = ScreenHelpers::GetScreenResolution() / ScreenHelpers::GetDpu() / 2.f;
    
    ProjectionMatrix = glm::perspectiveFov(FieldOfView, viewportSize.x, viewportSize.y, ZNear, ZFar);
    ViewMatrix = glm::translate(glm::mat4x4(), -Position);
}
