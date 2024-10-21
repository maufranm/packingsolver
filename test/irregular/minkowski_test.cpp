#include "irregular/minkowski.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;

namespace
{

    Shape build_polygon_shape(const std::vector<Point>& points)
    {
        Shape shape;
        ElementPos pos_prev = points.size() - 1;
        for (ElementPos pos = 0; pos < (ElementPos)points.size(); ++pos) {
            ShapeElement element;
            element.type = ShapeElementType::LineSegment;
            element.start = points[pos_prev];
            element.end = points[pos];
            shape.elements.push_back(element);
            pos_prev = pos;
        }
        return shape;
    }
    // get_poly
    // est ce que je retourne un poly ? 
    // est ce que ses points sont bien ceux que je veux ?
    
    // NFP
    // est ce que le nfp retourné est un poly ?
    // est ce que ce sont les bons points qui sont retournés ?
}

TEST(Irregular, ShapeToPolygon)
{
    Shape shape = build_polygon_shape({{2,2},{3,2},{3,3},{2,3}});
    Polygon_2 polygon = get_poly(shape);

    ASSERT_TRUE((std::is_same<decltype(polygon), Polygon_2>::value));
    EXPECT_EQ(polygon[0], Point_2(2,3));    //by shape construction
    EXPECT_EQ(polygon[1], Point_2(2,2));
    EXPECT_EQ(polygon[2], Point_2(3,2));
    EXPECT_EQ(polygon[3], Point_2(3,3));
}

//memory problem with this one
TEST(Irregular, NFP)
{
    Shape shapeFixed = build_polygon_shape({{3,2},{3,3},{2,3},{2,2}});
    Shape shapeMobile = build_polygon_shape({{-3,1},{-3,2},{-4,2},{-4,1}});

    Polygon_with_holes_2 poly = NFP(shapeFixed, shapeMobile);
    Polygon_2 outerPoly = poly.outer_boundary();

    EXPECT_EQ(outerPoly[0], Point_2(1,1));
    EXPECT_EQ(outerPoly[1], Point_2(3,1));
    EXPECT_EQ(outerPoly[2], Point_2(3,3));
    EXPECT_EQ(outerPoly[3], Point_2(1,3));
}