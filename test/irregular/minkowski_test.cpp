#include "irregular/minkowski.hpp"
#include <typeinfo> // Nécessaire pour typeid


#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

using namespace packingsolver;
using namespace packingsolver::irregular;
namespace fs = boost::filesystem;

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
    
    // get shape
    // est-ce que je retourne une shape
    // est ce que j'ai les bons points ?

    // neg
    // est-ce que j'ai les bons points ?
    // je retourne bien un poly ?

    // NFP
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

TEST(Irregular, PolygonToShape)
{
    Polygon_2 p;
    p.push_back(Point_2(2,3));
    p.push_back(Point_2(2,2));
    p.push_back(Point_2(3,2));
    p.push_back(Point_2(3,3));

    Shape shape = get_shape(p);
    

    ASSERT_TRUE((std::is_same<decltype(get_shape(p)), Shape>::value));
    EXPECT_EQ(shape.elements[0].start.x, 2);
    EXPECT_EQ(shape.elements[0].start.y, 3);    
    EXPECT_EQ(shape.elements[1].start.x, 2);
    EXPECT_EQ(shape.elements[1].start.y, 2);
    EXPECT_EQ(shape.elements[2].start.x, 3);
    EXPECT_EQ(shape.elements[2].start.y, 2);
    EXPECT_EQ(shape.elements[3].start.x, 3);
    EXPECT_EQ(shape.elements[3].start.y, 3);
}

TEST(Irregular, Negative)
{
    Polygon_2 p;
    p.push_back(Point_2(2,3));
    p.push_back(Point_2(2,2));
    p.push_back(Point_2(3,2));
    p.push_back(Point_2(3,3));

    Polygon_2 pn = negative_polygon(p);

    EXPECT_EQ(pn[0], Point_2(-2,-3));    //by shape construction
    EXPECT_EQ(pn[1], Point_2(-2,-2));
    EXPECT_EQ(pn[2], Point_2(-3,-2));
    EXPECT_EQ(pn[3], Point_2(-3,-3));
}

TEST(Irregular, NFP)
{
    Shape shapeFixed = build_polygon_shape({{3,2},{3,3},{2,3},{2,2}});
    Shape shapeMobile = build_polygon_shape({{-3,1},{-3,2},{-4,2},{-4,1}});

    Polygon_2 poly = NFP(shapeFixed, shapeMobile).outer_boundary();

    std::cout << poly;
}

TEST(Irregular, Intersection)
{
    Shape shape_1 = build_polygon_shape({{1,0},{1,1},{0,1}, {0,0}});
    Polygon_2 polygon_1 = get_poly(shape_1);


    Shape shape_2 = build_polygon_shape({{0,1},{1,1},{0,0}});
    Polygon_2 polygon_2 = get_poly(shape_2);

    Polygon_with_holes_2 NFP_2 = NFP(polygon_1, polygon_2);
    Polygon_with_holes_2 NFP_1 = NFP(polygon_1, polygon_1);

    EXPECT_TRUE(is_intersected(polygon_1, Point_2(0,0), polygon_1, Point_2(0,0), NFP_1));
    EXPECT_TRUE(is_intersected(polygon_1, Point_2(0,0.5), polygon_2, Point_2(0,1), NFP_2));
    EXPECT_TRUE(is_intersected(polygon_1, Point_2(0,0), polygon_2, Point_2(0,0), NFP_2));
    EXPECT_FALSE(is_intersected(polygon_1, Point_2(0,0), polygon_2, Point_2(0,-1), NFP_2));
    EXPECT_FALSE(is_intersected(polygon_1, Point_2(0,0), polygon_2, Point_2(1,1), NFP_2));
    EXPECT_TRUE(is_intersected(polygon_1, Point_2(50,2), polygon_2, Point_2(50,2), NFP_2));
}



TEST(oui, fi)
{
    std::cout << "test_print";
    //typeid(fs::path("data") / "irregular" / "tests" / "rectangles_non_guillotine.json").name() << " "
    //<< fs::path("data") / "irregular" / "tests" / "rectangles_non_guillotine.json";
}