#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

namespace pixelboost
{

Camera::Camera()
	: Position(0,0)
{

}

void Camera::ApplyTransform()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glScalef(ScreenHelpers::GetWorldScale()[0] * ScreenHelpers::GetAspectRatio(), ScreenHelpers::GetWorldScale()[1], 1.f);
	
    glTranslatef(-Position[0], -Position[1], 0.f);
}

}
