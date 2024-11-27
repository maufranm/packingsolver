#include "irregular/glouton.hpp"
#include "packingsolver/irregular/instance_builder.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;

TEST(Irregular, randomPointInShape)
{
    Shape container = build_polygon_shape({{1,1},{0,1},{5,5},{5,6}});

    Point_2 truc = random_point_in_shape(container);

    //vérifier égal à (-1,-1) -> pour afficher notre nombre
    //vérif dans rectangle (méthode annexes pour trouver rectangle)

    std::cout << "Test" << truc;

    EXPECT_EQ(truc, Point_2(-1,-1));
}

/*TEST(Irregular, glouton)
{   
    Shape shape;
    ShapeElement shape_element;
    shape_element.type = ShapeElementType::LineSegment;
    shape_element.start = {0, 0};
    shape_element.end = {10, 0};
    shape.elements.push_back(shape_element);
    shape_element.start = {10, 0};
    shape_element.end = {10, 10};
    shape.elements.push_back(shape_element);
    shape_element.start = {10, 10};
    shape_element.end = {0, 10};
    shape.elements.push_back(shape_element);
    shape_element.start = {0, 10};
    shape_element.end = {0, 0};
    shape.elements.push_back(shape_element);  //carré droit 10 par 10

    ItemShape item_shape;
    item_shape.shape = shape;

    Shape container = build_polygon_shape({{0,0},{100,0},{100,100},{0,100}});

    InstanceBuilder instance_builder;
    instance_builder.set_objective(packingsolver::Objective::VariableSizedBinPacking);  //useful?
    instance_builder.add_item_type({item_shape}, 1, 10);
    instance_builder.add_bin_type(container, -1, 1);
    const Instance instance = instance_builder.build();

    glouton(instance);
}*/