#pragma once

#include "pixelboost/math/maths.h"

namespace pixelboost
{

class Camera
{
public:
	Camera();
	void ApplyTransform();
	
	Vec2 Position;
};

}
