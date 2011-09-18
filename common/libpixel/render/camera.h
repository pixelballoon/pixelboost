#ifndef LIBPIXEL__COMMON__RENDER__CAMERA__H
#define LIBPIXEL__COMMON__RENDER__CAMERA__H

#include "svl/SVL.h"

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

#endif
