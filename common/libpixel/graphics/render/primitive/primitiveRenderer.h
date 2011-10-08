#ifndef LIBPIXEL__COMMON__GRAPHICS__RENDER__PRIMITIVE__PRIMITIVERENDERER__H
#define LIBPIXEL__COMMON__GRAPHICS__RENDER__PRIMITIVE__PRIMITIVERENDERER__H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "libpixel/math/maths.h"

namespace libpixel
{

class PrimitiveRenderer
{
public:
    PrimitiveRenderer();
    ~PrimitiveRenderer();
    
    void RenderEllipse(Vec2 position, Vec2 size, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec3 colour = Vec3(1.f, 1.f, 1.f));
    void RenderLine(Vec2 start, Vec2 end, Vec3 colour = Vec3(1.f, 1.f, 1.f));
    void RenderBox(Vec2 position, Vec2 size, Vec3 colour = Vec3(1.f, 1.f, 1.f));
};
    
}

#endif