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
    const ShapeElement& shape_el)   //to avoid copies
{
    return Point_2(shape_el.start.x,shape_el.start.y);
}

//from shape to polygon2
Polygon_2 irregular::get_poly(
    const Shape& s) 
{
    Polygon_2 pgn;
    // need points of the Shape
    int n = s.elements.size();
    for (int j = 0; j<s.elements.size() ; j++)
    {
        pgn.push_back(get_point(s.elements[j]));
    }
    return pgn;
}

//from poly to shape
Shape irregular::get_shape(
    const Polygon_2& p)
{
    Shape shape;
    for (ElementPos pos = 0; pos < (ElementPos)p.size() - 1; ++pos) {
        ShapeElement elem;
        elem.type = ShapeElementType::LineSegment;
        elem.start = {p[pos].x(), p[pos].y()};
        elem.end = {p[pos+1].x(), p[pos+1].y()};
        shape.elements.push_back(elem);
    }
    //the last line between last and first
    ShapeElement elem;
    ElementPos posF = 0;
    ElementPos posL = p.size() - 1;
    elem.type = ShapeElementType::LineSegment;
    elem.start = {p[posL].x(), p[posL].y()};
    elem.end = {p[posF].x(), p[posF].y()};
    shape.elements.push_back(elem);

    return shape;
}

Polygon_2 irregular::negative_polygon(
    const Polygon_2& polygon)
{
    Polygon_2 newPoly;
    for (int j = 0; j < polygon.size(); j++) 
    {
        newPoly.push_back(Point_2(-polygon[j].x(), -polygon[j].y()));
    }
    return newPoly;
}

//poly to shape -> return shape instead or poly nfp & ifp

Polygon_with_holes_2 irregular::NFP( 
    const Shape& shapeFixed, const Shape& shapeMobile)
{
    Polygon_2 polyFixed = get_poly(shapeFixed);
    Polygon_2 polyMobile = get_poly(shapeMobile);

    Polygon_2 polyMobileNeg = negative_polygon(polyMobile);  // "P" to "-P"
    
    Polygon_with_holes_2 polyMinkowski = minkowski_sum_by_full_convolution_2(polyFixed, polyMobileNeg);
    return polyMinkowski;
    }

/*
Polygon_with_holes_2 irregular::IFP( 
    const Shape& shapeContainer, const Shape& shapeMobile)
{
    Polygon_2 container = get_poly(shapeContainer);
    container.reverse_orientation();  // ext(container) = container.reverse_orientation()
    Polygon_2 polyMobile = get_poly(shapeMobile);
    Polygon_2 polyMobileNeg = negative_polygon(polyMobile);  // "P" to "-P"
    Polygon_2 refPoint;
    refPoint.push_back(polyMobile[0]);
    Polygon_with_holes_2 polyMinkowski = minkowski_sum_by_full_convolution_2(container, polyMobile);
    return minkowski_sum_by_full_convolution_2(polyMinkowski.outer_boundary(), refPoint).reverse_orientation();
}*/