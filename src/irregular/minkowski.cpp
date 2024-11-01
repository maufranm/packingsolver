#include "packingsolver/irregular/instance.hpp"
#include "irregular/minkowski.hpp"
#include <CGAL/minkowski_sum_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;

using namespace packingsolver;
using namespace packingsolver::irregular;

Point_2 irregular::get_point(
    ShapeElement shape_el) 
{
    return Point_2(shape_el.start.x,shape_el.start.y);
}

//from shape to polygon2
Polygon_2 irregular::get_poly(
    Shape s) 
{
    // need points of the Shape
    int n = s.elements.size();
    Point_2* points = new Point_2[n];
    int i = 0;
    for (int j = 0; j<=s.elements.size() ; j++, i++)
    {
        points[i] = get_point(s.elements[j]);
    }
    // we have all points
    Polygon_2 pgn(points, points+n);
    return pgn;
}

Polygon_with_holes_2 irregular::NFP( 
    Shape shapeFixed, Shape shapeMobile)
{
    Polygon_2 polyFixed = get_poly(shapeFixed);
    Polygon_2 polyMobile = get_poly(shapeMobile);
    polyMobile.reverse_orientation();  // "-P" = P.reverse_orientation() ?
    return minkowski_sum_by_full_convolution_2(polyFixed, polyMobile);  //ajouter polyMobile[0] ?
}

Polygon_with_holes_2 irregular::IFP( 
    Shape shapeContainer, Shape shapeMobile)
{
    Polygon_2 container = get_poly(shapeContainer);  // ext(container) = container.reverse_orientation()  ?
    container.reverse_orientation();
    Polygon_2 polyMobile = get_poly(shapeMobile);
    // polyMobile.reverse_orientation();  // "-P" = P.reverse_orientation()
    minkowski_sum_by_full_convolution_2(container, polyMobile);  //ajouter polyMobile[0] ?
}


bool irregular::is_inside( Point_2 point, Polygon_with_holes_2 polygon)
{
   return true
}