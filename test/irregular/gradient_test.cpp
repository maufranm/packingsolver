#include "irregular/gradient.hpp"
#include "irregular/minkowski.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;


TEST(kkkkkkkkkkkkkkkkkkkkkkkk, popopopoppopopopopo)
{
    std::cout << 'oui';
}


TEST(Irregular, overlap)
{
    Polygon_2 poly1;
    p.push_back(Point_2(0,0));
    p.push_back(Point_2(0,4));
    p.push_back(Point_2(4,4));
    p.push_back(Point_2(4,0));
    Polygon_2 poly2=poly1;

    Polygon_with_holes_2 = NFP(poly1,poly2);

    Point_2 emplacement1 = Point_2(0,0);
    Point_2 emplacement2 = Point_2(2,3);

    auto truc = overlap(poly1, emplacement1, poly2, emplacement2, NFP);
    std::tuple< Vect_2, double> solution = { Vect_2(0,1), 1.0};

    EXPECT_EQ(truc,solution)
}

TEST(Irregular, norm)
{
    EXPECT_EQ(norm(Vect_2(0.0 , 0.0)), 0.0);
    EXPECT_EQ(norm(Vect_2(3 , 4)), 5);
    EXPECT_EQ(norm(Vect_2(1 , 1)), sqrt(2));
}

TEST(Irregular, orthogonal_projection)
{   
    Vect_2 a =orthogonal_projection( Point_2(0,0), Point_2(0,3), Point_2(1,1) )
    EXPECT_EQ( a , Vect_2(0,-1));

    Vect_2 b =orthogonal_projection( Point_2(1,1), Point_2(3,3), Point_2(1,3) )
    EXPECT_EQ( b , Vect_2(1,-1));    

    Vect_2 c =orthogonal_projection( Point_2(0,0), Point_2(5,5), Point_2(10,10) )
    EXPECT_EQ( c , Vect_2(0,0));
}


/*
cd ~/packingsolver

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release --parallel
cmake --install build --config Release --prefix install

cd build/test/irregular
ctest --output-on-failure --parallel
*/