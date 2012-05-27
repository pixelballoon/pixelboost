#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL2

#include <OpenGL/gl.h>

using namespace pb;

Camera::Camera()
{

}

Camera::~Camera()
{
    
}
    
OrthographicCamera::OrthographicCamera(Vec2 position, Vec2 scale)
    : Position(position)
    , Scale(scale)
{
    
}

void OrthographicCamera::ApplyTransform()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    glScalef(ScreenHelpers::GetWorldScale()[0] * ScreenHelpers::GetAspectRatio() * Scale[0], ScreenHelpers::GetWorldScale()[1] * Scale[1], 1.f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-Position[0], -Position[1], 0.f);
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

PerspectiveCamera::PerspectiveCamera()
    : FieldOfView(90.f)
    , Offset(0,0)
    , Position(0,0,0)
    , ZNear(0.1f)
    , ZFar(100.f)
{
}

void PerspectiveCamera::ApplyTransform()
{
    float aspect = ScreenHelpers::GetScreenResolution()[0]/ScreenHelpers::GetScreenResolution()[1];
    float fH = tan(FieldOfView/360*M_PI) * ZNear;
    float fW = fH * aspect;
    Vec2 offset = -Offset / ZFar;
    offset *= tan(FieldOfView/360*M_PI);
    offset *= aspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -fW + offset[0], fW + offset[0], -fH + offset[1], fH + offset[1], ZNear, ZFar );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-Position[0], -Position[1], Position[2]);
}

#endif
