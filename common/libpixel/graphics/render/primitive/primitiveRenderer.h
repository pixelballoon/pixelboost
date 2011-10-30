#pragma once

#include "libpixel/math/maths.h"

namespace libpixel
{

class PrimitiveRenderer
{
public:
    PrimitiveRenderer();
    ~PrimitiveRenderer();
    
    void RenderEllipse(Vec2 position, Vec2 size, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f), int segments = 12);
    void RenderLine(Vec2 start, Vec2 end, Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f));
    void RenderBox(Vec2 position, Vec2 size, Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f));
};
    
}
