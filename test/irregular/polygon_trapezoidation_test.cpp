#include "irregular/polygon_trapezoidation.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;

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

TEST(IrregularPolygonTrapezoidation, Triangle1)
{
    Shape shape = build_polygon_shape({{0, 0}, {3, 0}, {1, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);

    EXPECT_EQ(trapezoids.size(), 1);
    GeneralizedTrapezoid trapezoid_1(0, 3, 0, 3, 1, 1);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Triangle2)
{
    Shape shape = build_polygon_shape({{2, 0}, {3, 3}, {0, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 1);
    GeneralizedTrapezoid trapezoid_1(0, 3, 2, 2, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Square)
{
    Shape shape = build_polygon_shape({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);

    EXPECT_EQ(trapezoids.size(), 1);
    GeneralizedTrapezoid trapezoid_1(0, 1, 0, 1, 0, 1);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Trapezoid1)
{
    Shape shape = build_polygon_shape({{0, 0}, {3, 0}, {2, 3}, {1, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);

    EXPECT_EQ(trapezoids.size(), 1);
    GeneralizedTrapezoid trapezoid_1(0, 3, 0, 3, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Trapezoid2)
{
    Shape shape = build_polygon_shape({{1, 0}, {2, 0}, {3, 3}, {0, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);

    EXPECT_EQ(trapezoids.size(), 1);
    GeneralizedTrapezoid trapezoid_1(0, 3, 1, 2, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Triangle3)
{
    Shape shape = build_polygon_shape({{4, 0}, {1, 3}, {0, 1}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 2);
    GeneralizedTrapezoid trapezoid_1(1, 3, 0, 3, 1, 1);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(0, 1, 4, 4, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Trapezoid3)
{
    Shape shape = build_polygon_shape({{5, 0}, {2, 3}, {1, 3}, {0, 1}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 2);
    GeneralizedTrapezoid trapezoid_1(1, 3, 0, 4, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(0, 1, 5, 5, 0, 4);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, DoubleTrapezoid1)
{
    Shape shape = build_polygon_shape({{0, 0}, {4, 0}, {2, 2}, {4, 4}, {0, 4}, {1, 2}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 2);
    GeneralizedTrapezoid trapezoid_1(2, 4, 1, 2, 0, 4);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(0, 2, 0, 4, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, DoubleTrapezoid2)
{
    Shape shape = build_polygon_shape({{1, 0}, {2, 0}, {4, 2}, {2, 4}, {1, 4}, {0, 2}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 2);
    GeneralizedTrapezoid trapezoid_1(2, 4, 0, 4, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(0, 2, 1, 2, 0, 4);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, ReversedH)
{
    Shape shape = build_polygon_shape({
            {0, 0}, {3, 0}, {3, 1}, {2, 1}, {2, 2}, {3, 2},
            {3, 3}, {0, 3}, {0, 2}, {1, 2}, {1, 1}, {0, 1}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 3);
    GeneralizedTrapezoid trapezoid_1(2, 3, 0, 3, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(1, 2, 1, 2, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
    GeneralizedTrapezoid trapezoid_3(0, 1, 0, 3, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_3), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, Cross)
{
    Shape shape = build_polygon_shape({
            {1, 0}, {2, 0}, {2, 1}, {3, 1}, {3, 2}, {2, 2},
            {2, 3}, {1, 3}, {1, 2}, {0, 2}, {0, 1}, {1, 1}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 3);
    GeneralizedTrapezoid trapezoid_1(2, 3, 1, 2, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(1, 2, 0, 3, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
    GeneralizedTrapezoid trapezoid_3(0, 1, 1, 2, 1, 2);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_3), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, U)
{
    Shape shape = build_polygon_shape({
            {0, 0}, {3, 0}, {3, 3}, {2, 3},
            {2, 1}, {1, 1}, {1, 3}, {0, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    //for (const GeneralizedTrapezoid& trapezoid: trapezoids)
    //    std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 3);
    GeneralizedTrapezoid trapezoid_1(1, 3, 0, 1, 0, 1);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(1, 3, 2, 3, 2, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
    GeneralizedTrapezoid trapezoid_3(0, 1, 0, 3, 0, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_3), trapezoids.end());
}

TEST(IrregularPolygonTrapezoidation, W)
{
    Shape shape = build_polygon_shape({
            {0, 0}, {5, 0}, {5, 3}, {4, 3},
            {4, 1}, {3, 1}, {3, 2}, {2, 2},
            {2, 1}, {1, 1}, {1, 3}, {0, 3}});
    std::vector<GeneralizedTrapezoid> trapezoids = polygon_trapezoidation(shape);
    for (const GeneralizedTrapezoid& trapezoid: trapezoids)
        std::cout << trapezoid << std::endl;

    EXPECT_EQ(trapezoids.size(), 4);
    GeneralizedTrapezoid trapezoid_1(1, 3, 0, 1, 0, 1);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_1), trapezoids.end());
    GeneralizedTrapezoid trapezoid_2(1, 3, 4, 5, 4, 5);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_2), trapezoids.end());
    GeneralizedTrapezoid trapezoid_3(1, 2, 2, 3, 2, 3);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_3), trapezoids.end());
    GeneralizedTrapezoid trapezoid_4(0, 1, 0, 5, 0, 5);
    EXPECT_NE(std::find(trapezoids.begin(), trapezoids.end(), trapezoid_4), trapezoids.end());
}
