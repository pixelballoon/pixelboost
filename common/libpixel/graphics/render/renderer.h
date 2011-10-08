#ifndef LIBPIXEL_COMMON__GRAPHICS__RENDER__RENDERER__H
#define LIBPIXEL_COMMON__GRAPHICS__RENDER__RENDERER__H

#include "libpixel/graphics/device/device.h"

namespace libpixel
{

class Vertex
{
public:
    GLfloat _Position[3];
    GLfloat _Normal[3];
    GLfloat _UV[2];
};

}

#endif