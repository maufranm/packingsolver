#include "irregular/glouton.hpp"
#include "packingsolver/irregular/instance_builder.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;

TEST(Irregular, randomPointInShape)
{
    Shape container = build_polygon_shape({{1,1},{0,1},{5,5},{5,6}});

    auto [x_min, x_max, y_min, y_max] = calculateBounds(container);
    Point_2 truc = random_point_in_shape(container);

    //vérifier égal à (-1,-1) -> pour afficher notre nombre
    //vérif dans rectangle (méthode annexes pour trouver rectangle)

    std::cout << "Test" << truc;

    EXPECT_NE(truc, Point_2(-1,-1));
    EXPECT_GE(truc.x(), x_min);
    EXPECT_GE(truc.y(), y_min);
    EXPECT_LE(truc.x(), x_max);
    EXPECT_LE(truc.y(), y_max);
}

TEST(Irregular, glouton)
{   
    Shape shape = build_polygon_shape({{0,0},{10,0},{10,10},{0,10}}); //carré droit 10 par 10
    // Shape shape;
    // ShapeElement shape_element;
    // shape_element.type = ShapeElementType::LineSegment;
    // shape_element.start = {0, 0};
    // shape_element.end = {10, 0};
    // shape.elements.push_back(shape_element);
    // shape_element.start = {10, 0};
    // shape_element.end = {10, 10};
    // shape.elements.push_back(shape_element);
    // shape_element.start = {10, 10};
    // shape_element.end = {0, 10};
    // shape.elements.push_back(shape_element);
    // shape_element.start = {0, 10};
    // shape_element.end = {0, 0};
    // shape.elements.push_back(shape_element);  //carré droit 10 par 10

    ItemShape item_shape;
    item_shape.shape = shape;

    // Shape container = build_polygon_shape({{0,0},{100,0},{100,100},{0,100}}); //bugged
    // Shape container = build_polygon_shape({{0,0},{30,0},{30,30},{0,30}}); //bugged
    // Shape container = build_polygon_shape({{0,0},{50,0},{30,25}}); //bugged
    // Shape container = build_polygon_shape({{0,0},{30,25},{50,0}}); //bugged
    // Shape container = build_polygon_shape({{0,1},{99,0},{100,49},{1,50}});
    Shape container = build_polygon_shape({{0,0},{20,20},{0,40},{-20,20}});

    InstanceBuilder instance_builder;
    instance_builder.set_objective(packingsolver::Objective::Knapsack);  //useful?
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_bin_type(container, -1, 1);
    const Instance instance = instance_builder.build();

    glouton(instance);
    EXPECT_EQ(true,false);
}