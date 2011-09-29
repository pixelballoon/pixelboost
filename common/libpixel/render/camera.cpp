#include "libpixel/logic/game.h"
#include "libpixel/render/camera.h"
#include "libpixel/render/gl.h"
#include "libpixel/render/screenHelpers.h"

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
    
    glScalef(ScreenHelpers::GetWorldScale()[0] * ScreenHelpers::GetAspectRatio(), ScreenHelpers::GetWorldScale()[1], 0.f);
	
    if (Game::Instance()->IsLandscape())
        glRotatef(90, 0, 0, 1);
    
    glTranslatef(Position[0], Position[1], 0.f);
}

}