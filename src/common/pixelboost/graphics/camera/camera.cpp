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
    
OrthographicCamera::OrthographicCamera()
    : Position(0,0)
{
    
}

void OrthographicCamera::ApplyTransform()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glScalef(ScreenHelpers::GetWorldScale()[0] * ScreenHelpers::GetAspectRatio(), ScreenHelpers::GetWorldScale()[1], 1.f);
	
    glTranslatef(-Position[0], -Position[1], 0.f);
}

PerspectiveCamera::PerspectiveCamera()
    : Position(0,0,0)
    , FieldOfView(90.f)
    , ZNear(0.1f)
    , ZFar(100.f)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
}

void PerspectiveCamera::ApplyTransform()
{
    glMatrixMode(GL_PROJECTION);
    float matrix[16];
    BuildMatrix(matrix, FieldOfView, ZNear, ZFar);
    glLoadMatrixf(matrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-Position[0], -Position[1], -Position[2]);
}

// Code adapted from http://www.geeks3d.com/20090729/howto-perspective-projection-matrix-in-opengl/
void PerspectiveCamera::BuildMatrix(float* m, float fov, float znear, float zfar)
{
    float aspect = ScreenHelpers::GetScreenResolution()[0]/ScreenHelpers::GetScreenResolution()[1];
    
    float xymax = znear * tan(fov * M_PI/360.f);
    float ymin = -xymax;
    float xmin = -xymax;
    
    float width = xymax - xmin;
    float height = xymax - ymin;
    
    float depth = zfar - znear;
    float q = -(zfar + znear) / depth;
    float qn = -2 * (zfar * znear) / depth;
    
    float w = 2 * znear / width;
    w = w / aspect;
    float h = 2 * znear / height;
    
    m[0]  = w;
    m[1]  = 0;
    m[2]  = 0;
    m[3]  = 0;
    
    m[4]  = 0;
    m[5]  = h;
    m[6]  = 0;
    m[7]  = 0;
    
    m[8]  = 0;
    m[9]  = 0;
    m[10] = q;
    m[11] = -1;
    
    m[12] = 0;
    m[13] = 0;
    m[14] = qn;
    m[15] = 0;
}

#endif
