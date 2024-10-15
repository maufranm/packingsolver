#include "packingsolver/irregular/instance.hpp"
#include "irregular/test.hpp"
#include <CGAL/minkowski_sum_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;

namespace packingsolver
{
namespace irregular
{ 
Point_2 get_point(ShapeElement shape_el) {
    return Point_2(shape_el.start.x,shape_el.start.y);
}

Polygon_2 get_poly(Shape s) {
    // dabord il faut des points avec get_point
    int n = s.elements.size();
    Point_2* points = new Point_2[n];
    int i = 0;
    for (ShapeElement element = s.elements.begin(); element!=s.elements.end() ; element++, i++)
    {
        points[i] = get_point(element);
    }
    // we have all points
    Polygon_2 pgn(points, points+n);
    return pgn;
}

Polygon_with_holes_2 NFP( Shape shapeFixed, Shape shapeMobile){
    Polygon_2 polyFixed = get_poly(shapeFixed);
    Polygon_2 polyMobile = get_poly(shapeMobile);
    return minkowski_sum_by_full_convolution_2(polyFixed, polyMobile);
}





int main()
{    
}

}
}