#include "packingsolver/irregular/instance.hpp"
#include <CGAL/minkowski_sum_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;

namespace packingsolver
{

namespace irregular
{

Point_2 get_point(
    ShapeElement shape_el);

Polygon_2 get_poly(
    Shape s);

Polygon_with_holes_2 NFP( 
    Shape shapeFixed, Shape shapeMobile);


Polygon_with_holes_2 IFP(
    Shape shapeContainer, Shape shapeMobile);
}

}
