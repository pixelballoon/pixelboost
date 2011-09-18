#ifndef LIBPIXEL_COMMON__RENDER__RENDERER__H
#define LIBPIXEL_COMMON__RENDER__RENDERER__H

#include "libpixel/render/gl.h"

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