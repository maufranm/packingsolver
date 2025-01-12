#include "irregular/gradient.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::irregular;


TEST(kkkkkkkkkkkkkkkkkkkkkkkk, popopopoppopopopopo)
{
    std::cout << 'oui';
}
TEST(Irregular, overlap)
{
    
}

TEST(Gradient, Poly2Edges)
{
    Polygon_2 p;
    p.push_back(Point_2(2,3));
    p.push_back(Point_2(2,2));
    p.push_back(Point_2(3,2));
    p.push_back(Point_2(3,3));
    
    std::vector< std::pair<Point_2, Point_2> > edges = get_edges_polygon(p);

    EXPECT_EQ(edges.size(), 4);
    EXPECT_EQ(edges[0], std::make_pair(CGAL::Point_2<CGAL::Epick>(2, 3), CGAL::Point_2<CGAL::Epick>(2, 2)));
}