#include "packingsolver/irregular/instance.hpp"
#include <CGAL/minkowski_sum_2.h>

// typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;

namespace packingsolver
{

namespace irregular
{

Point_2 get_point(
    const ShapeElement& shape_el);

Polygon_2 get_poly(
    const Shape& s);

Shape get_shape(
    const Polygon_2& p);

Polygon_2 negative_polygon(
    const Polygon_2& polygon);

Polygon_with_holes_2 NFP(
    const Shape& shapeFixed, const Shape& shapeMobile);

Polygon_with_holes_2 NFP( 
    const Polygon_2& polyFixed, const Polygon_2& polyMobile);

bool is_intersected(
    Polygon_2 poly1, Point_2 emplacement1, Polygon_2 poly2, Point_2 emplacement2, Polygon_with_holes_2 NFP);    
}
}