#include "libpixel/graphics/camera/camera.h"
#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/helper/screenHelpers.h"

namespace libpixel
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
