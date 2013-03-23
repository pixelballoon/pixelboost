#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace pb
{

    class HermiteCurve1D
    {
    public:
        float Evaluate(float t);
        
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

    class HermiteCurve2D
    {
    public:
        class Point
        {
        public:
            Point(const glm::vec2& position, const glm::vec2& controlIn = glm::vec2(0,0), const glm::vec2& controlOut = glm::vec2(0,0));
            
        public:
            glm::vec2 ControlIn;
            glm::vec2 Position;
            glm::vec2 ControlOut;
            
            float Parameterization;
        };

    public:
        glm::vec2 Evaluate(float t) const;
        glm::vec2 EvaluateParam(float t) const;
        float GetArcLength() const;
        
        void AddPoint(const Point& point);
        void Parameterize();
       
    private:
        glm::vec2 Evaluate(const Point& a, const Point& b, float u) const;
        
        std::vector<Point> _Points;
        float _ArcLength;
    };

}
