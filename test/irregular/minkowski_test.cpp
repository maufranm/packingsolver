#include "irregular/minkowski.hpp"
#include "irregular/glouton.hpp"
#include "packingsolver/irregular/instance_builder.hpp"

#include <gtest/gtest.h>
#include <typeinfo> // Nécessaire pour typeid
#include <boost/filesystem.hpp>

using namespace packingsolver;
using namespace packingsolver::irregular;
namespace fs = boost::filesystem;

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

    std::cerr << poly << std::endl;
    
    // shape 13 of atris1240 (indexed from 0)
    Polygon_2 shape13 = get_poly(build_polygon_shape(
        {{211700, 0}, {211700, 216000}, {312900, 216000}, {312900, 492900},
     {211700, 492900}, {211700, 280000}, {104500, 280000}, {104500, 417300},
      {0, 417300}, {0, 174700}, {104500, 174700}, {104500, 0}}
     ));
    // Polygon_2 shape10 = get_poly(build_polygon_shape(
        // {{96500, 470000}, {96500, 269000}, {0, 269000}, {0, 0},
        // {96500, 0}, {96500, 208600}, {188700, 208600}, {188700, 126500},
        // {298700, 126500}, {298700, 311800}, {188700, 311800}, {188700, 470000}}
        // ));
    Polygon_2 shape13_neg = negative_polygon(shape13);
    
    // shape 1 of atris1240
    Polygon_2 shape1 = get_poly(build_polygon_shape(
        {{307900, 0}, {307900, 239900}, {201500, 239900}, {201500, 375700},
         {105000, 375700}, {105000, 572200}, {0, 572200}, {0, 299100},
         {105000, 299100}, {105000, 176800}, {201500, 176800}, {201500, 0}}
        ));
    // Polygon_2 shape7 = get_poly(build_polygon_shape(
    //     {{465200, 194100}, {271000, 194100}, {271000, 303400}, {38300, 303400},
    //     {38300, 194100}, {211100, 194100}, {211100, 92100}, {0, 92100},
    //     {0, 0}, {268900, 0}, {268900, 92100}, {465200, 92100}}
    //     ));

    Polygon_with_holes_2 polyMinkowski;

    std::cerr << "Entering minkowski_sum_by_full_convolution_2 no negative poly" << std::endl;
    std::cerr << shape13 << std::endl;
    polyMinkowski = minkowski_sum_by_full_convolution_2(shape1, shape13);
    std::cerr << "Entering minkowski_sum_by_full_convolution_2 with negative poly" << std::endl;
    std::cerr << shape13_neg << std::endl;
    
    polyMinkowski = minkowski_sum_by_full_convolution_2(shape1, shape13_neg);
    std::cerr << polyMinkowski << std::endl;
    Polygon_2 poly = NFP(shape1, shape13 ).outer_boundary();

    std::cerr << poly;
    EXPECT_EQ(true,false);

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
    //EXPECT_FALSE(true);
    std::cout << "test_print";
    //typeid(fs::path("data") / "irregular" / "tests" / "rectangles_non_guillotine.json").name() << " "
    //<< fs::path("data") / "irregular" / "tests" / "rectangles_non_guillotine.json";
}