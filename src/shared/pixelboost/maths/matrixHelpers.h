#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace pb
{

enum RotationOrder
{
	kRotationOrder_XYZ,
};

inline glm::mat4x4 CreateRotationMatrix(RotationOrder rotationOrder, glm::vec3 rotation)
{
	glm::mat4x4 transform;

	switch (rotationOrder)
	{
		case kRotationOrder_XYZ:
			transform = glm::rotate(transform, rotation.x, glm::vec3(1,0,0));
			transform = glm::rotate(transform, rotation.y, glm::vec3(0,1,0));
			transform = glm::rotate(transform, rotation.z, glm::vec3(0,0,1));
		break;
	}
	
	return transform;
}

inline glm::mat4x4 CreateTransformMatrix(RotationOrder rotationOrder, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	glm::mat4x4 transform;

	transform = glm::translate(transform, translate);
	transform = transform * CreateRotationMatrix(rotationOrder, rotate);
	transform = glm::scale(transform, scale);

	return transform;
}

}
