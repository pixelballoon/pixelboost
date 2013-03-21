#include "glm/gtx/spline.hpp"

#include "pixelboost/maths/hermiteCurve.h"

using namespace pb;

float HermiteCurve1D::Evaluate(float x)
{
    return glm::hermite(Points[0].Position, Points[0].ControlOut, Points[1].Position, -Points[1].ControlIn, x).y;
}

HermiteCurve1D::Point::Point(const glm::vec2& controlIn, const glm::vec2& position, const glm::vec2& controlOut)
    : ControlIn(controlIn)
    , Position(position)
    , ControlOut(controlOut)
{
    
}

HermiteCurve2D::Point::Point(const glm::vec2& position, const glm::vec2& controlIn, const glm::vec2& controlOut)
    : ControlIn(controlIn)
    , Position(position)
    , ControlOut(controlOut)
    , Parameterization(0)
{
    
}

glm::vec2 HermiteCurve2D::Evaluate(float t) const
{
    // TODO : Assert if _Points is empty, no assert in shared!
    
    int first = glm::floor(t * _Points.size());
    
    if (first < 0)
    {
        return _Points[0].Position;
    } else if (first >= _Points.size()-1)
    {
        return _Points[_Points.size()-1].Position;
    }
    
    float u = (t*_Points.size()) - first;
    
    return Evaluate(_Points[first], _Points[first+1], u);
}

glm::vec2 HermiteCurve2D::EvaluateParam(float t) const
{
    // TODO : Assert if _Points is empty, no assert in shared!
    
    if (t <= 0)
    {
        return _Points[0].Position;
    } else if (t >= _ArcLength)
    {
        return _Points[_Points.size()-1].Position;
    }
    
    for (int i=0; i<_Points.size()-1; i++)
    {
        if (t < _Points[i+1].Parameterization)
        {
            return Evaluate(_Points[i], _Points[i+1], (t-_Points[i].Parameterization)/(_Points[i+1].Parameterization-_Points[i].Parameterization));
        }
    }
    
    return _Points[0].Position;
}

float HermiteCurve2D::GetArcLength() const
{
    return _ArcLength;
}

void HermiteCurve2D::AddPoint(const Point& point)
{
    _Points.push_back(point);
}

void HermiteCurve2D::Parameterize()
{
    _ArcLength = 0.f;
    
    if (!_Points.size())
        return;
    
    for (int pointIndex=0; pointIndex<_Points.size()-1; pointIndex++)
    {
        glm::vec2 prev = _Points[pointIndex].Position;
        for (float u=0.1f; u<=1.f; u+=0.1f)
        {
            glm::vec2 point = Evaluate(_Points[pointIndex], _Points[pointIndex+1], u);
            
            _ArcLength += glm::distance(prev, point);
            
            prev = point;
        }
        
        _Points[pointIndex+1].Parameterization = _ArcLength;
    }
}

glm::vec2 HermiteCurve2D::Evaluate(const Point& a, const Point& b, float u) const
{
    return glm::hermite(a.Position, a.ControlOut, b.Position, -b.ControlIn, u);
}

