#pragma once

#include "libpixel/math/maths.h"

namespace libpixel
{

class Camera
{
public:
	Camera();
	void ApplyTransform();
	
	Vec2 Position;
};

}
