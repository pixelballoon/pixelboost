#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace pb
{

enum RotationOrder
{
	kRotationOrder_XYZ,
};

inline glm::mat4x4 CreateTranslateMatrix(glm::vec3 translate, glm::mat4x4 transform = glm::mat4x4())
{
    return glm::translate(transform, translate);
}

inline glm::mat4x4 CreateRotateMatrix(RotationOrder rotationOrder, glm::vec3 rotation, glm::mat4x4 transform = glm::mat4x4())
{
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

inline glm::mat4x4 CreateScaleMatrix(glm::vec3 scale, glm::mat4x4 transform = glm::mat4x4())
{
    return glm::scale(transform, scale);
}

inline glm::mat4x4 CreateTransformMatrix(RotationOrder rotationOrder, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	glm::mat4x4 transform;

	transform = CreateTranslateMatrix(translate, transform);
	transform = CreateRotateMatrix(rotationOrder, rotate, transform);
	transform = CreateScaleMatrix(scale, transform);

	return transform;
}

}
