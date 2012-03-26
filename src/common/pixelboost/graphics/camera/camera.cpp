#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

using namespace pixelboost;

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
    glOrthof(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    glScalef(ScreenHelpers::GetWorldScale()[0] * ScreenHelpers::GetAspectRatio() * Scale[0], ScreenHelpers::GetWorldScale()[1] * Scale[1], 1.f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-Position[0], -Position[1], 0.f);
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
    glFrustumf( -fW + offset[0], fW + offset[0], -fH + offset[1], fH + offset[1], ZNear, ZFar );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-Position[0], -Position[1], Position[2]);
}

#endif
