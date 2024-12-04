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

    ItemShape item_shape;
    item_shape.shape = shape;

    Shape container = build_polygon_shape({{0,0},{100,0},{100,100},{0,100}});

    InstanceBuilder instance_builder;
    instance_builder.set_objective(packingsolver::Objective::VariableSizedBinPacking);  //useful?
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1); //duplicate shape (warning: passed by reference)
    instance_builder.add_bin_type(container, -1, 1);
    const Instance instance = instance_builder.build();

    glouton(instance);
}