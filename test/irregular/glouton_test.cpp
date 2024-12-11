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
    Shape shape = build_polygon_shape({{0,0},{10,0},{10,10},{0,10}}); //carré droit 10 par 10, référence en haut à gauche


    ItemShape item_shape;
    item_shape.shape = shape;

    // Shape container = build_polygon_shape({{0,0},{100,0},{100,100},{0,100}}); //bugged boucle infini
    // Shape container = build_polygon_shape({{0,0},{30,0},{30,30},{0,30}});
    //Shape container = build_polygon_shape({{0,0},{50,0},{0,50}}); //same

    // Shape container = build_polygon_shape({{0,0},{50,0},{30,25}}); //bugged NUMBER OF BORDERS: 1 ; 50,0 0,0 30,30 et place rien...
    //Shape container = build_polygon_shape({{0,0},{50,50},{0,100}});  //same

    // Shape container = build_polygon_shape({{0,1},{99,0},{100,49},{1,50}});  //ok
    //Shape container = build_polygon_shape({{0,0},{50,50},{0,100},{-50,50}});  //ok
    Shape container = build_polygon_shape({{0,0},{50,50},{0,100},{-40,60},{-40,40}}); // OK (??? il y a un bord vertical)

    InstanceBuilder instance_builder;
    instance_builder.set_objective(packingsolver::Objective::Knapsack);  //useful?
    //instance_builder.add_item_type({item_shape}, 1, 30);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
    instance_builder.add_item_type({item_shape}, 1, 1);
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

/*
cmake --build build --config Release --parallel
cmake --install build --config Release --prefix install

cd build/test/irregular
ctest --output-on-failure --parallel
*/