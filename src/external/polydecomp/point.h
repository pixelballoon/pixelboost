#pragma once

#include <ostream>

#include "common.h"

namespace polydecomp
{
    class Point {
    public:
        Scalar x, y;

        Point();
        Point(std::ifstream& fin);
        Point(Scalar x, Scalar y);

        friend std::ostream & operator<<(std::ostream &os, const Point &p);
        friend Point operator+(const Point &a, const Point &b);
        friend Scalar area(const Point &a, const Point &b, const Point &c);
        friend bool left(const Point &a, const Point &b, const Point &c);
        friend bool leftOn(const Point &a, const Point &b, const Point &c);
        friend bool right(const Point &a, const Point &b, const Point &c);
        friend bool rightOn(const Point &a, const Point &b, const Point &c);
        friend bool collinear(const Point &a, const Point &b, const Point &c);
        friend Scalar sqdist(const Point &a, const Point &b);
    };
}
