#include "packingsolver/irregular/instance.hpp"
#include "packingsolver/irregular/solution.hpp"
#include "irregular/shape.hpp"
#include "irregular/minkowski.hpp"
#include <CGAL/minkowski_sum_2.h>

// typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Vector_2<CGAL::Epick> Vect_2;

namespace packingsolver
{

namespace irregular
{

std::vector< std::pair<Point_2, Point_2> > get_edges(
    Polygon_2 poly);

std::vector< std::pair<Point_2, Point_2>> get_edges(
    Polygon_with_holes_2 poly);

std::tuple< Vect_2, double> overlap( 
    Polygon_2 poly1, Point_2 emplacement1, Polygon_2 poly2, Point_2 emplacement2,  Polygon_with_holes_2 NFP);
}
}