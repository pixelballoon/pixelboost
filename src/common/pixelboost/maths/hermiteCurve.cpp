#include "glm/gtx/spline.hpp"

#include "pixelboost/maths/hermiteCurve.h"

using namespace pb;

float HermiteCurve2D::Evaluate(float x)
{
    return glm::hermite(Points[0].Position, Points[0].ControlOut, Points[1].Position, -Points[1].ControlIn, x).y;
}

HermiteCurve2D::Point::Point(const glm::vec2& controlIn, const glm::vec2& position, const glm::vec2& controlOut)
    : ControlIn(controlIn)
    , Position(position)
    , ControlOut(controlOut)
{
    
}
