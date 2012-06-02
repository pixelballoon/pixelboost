#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

using namespace pb;

Camera::Camera(glm::vec3 position, glm::vec3 rotation)
    : Position(position)
    , Rotation(rotation)
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
    Vec2 temp = ScreenHelpers::GetScreenResolution() / ScreenHelpers::GetDpu();
    glm::vec2 viewportSize = glm::vec2(temp[0], temp[1]) / 2.f;
    
    Projection = glm::ortho(-viewportSize.x, viewportSize.x, -viewportSize.y, viewportSize.y, ZNear, ZFar);
    ModelView = glm::translate(glm::mat4x4(), -Position);
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
    Vec2 temp = ScreenHelpers::GetScreenResolution() / ScreenHelpers::GetDpu();
    glm::vec2 viewportSize = glm::vec2(temp[0], temp[1]) / 2.f;
    
    Projection = glm::perspectiveFov(FieldOfView, viewportSize.x, viewportSize.y, ZNear, ZFar);
    ModelView = glm::translate(glm::mat4x4(), -Position);
}
