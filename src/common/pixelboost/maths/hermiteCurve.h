#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace pb
{

class HermiteCurve2D
{
public:
	float Evaluate(float x);
	
public:
	class Point
	{
    public:
        Point(const glm::vec2& controlIn, const glm::vec2& position, const glm::vec2& controlOut);
        
    public:
		glm::vec2 ControlIn;
		glm::vec2 Position;
		glm::vec2 ControlOut;
	};

	std::vector<Point> Points;
};

}
