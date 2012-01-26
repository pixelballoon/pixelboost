// Written by Mark Bayazit (darkzerox)
// March 23, 2009

// Adapted by Jamie Hales (pixelballoon.com)

#include <vector>
#include "point.h"

namespace polydecomp
{

class PolyDecomp
{
public:
    typedef std::vector<Point> Polygon;
    
public:
    PolyDecomp();
    
    const std::vector<Polygon>& Decompose(Polygon& polygon);
    
private:
    void makeCCW(Polygon &poly);
    bool isReflex(const Polygon &poly, const int &i);
    Point intersection(const Point &p1, const Point &p2, const Point &q1, const Point &q2);
    void swap(int &a, int &b);
    void decomposePoly(Polygon poly);

private:
    Polygon incPoly;
    bool polyComplete;
    std::vector<Polygon> polys;
    std::vector<Point> steinerPoints, reflexVertices;
};

}
