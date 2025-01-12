#include <stdexcept>
#include <tuple>
#include <cmath>
#include "packingsolver/irregular/instance.hpp"
#include "irregular/minkowski.hpp"
#include <random>
#include "irregular/glouton.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Vector_2<CGAL::Epick> Vect_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Polygon_with_holes_2::Hole_const_iterator HoleIterator;

namespace packingsolver
{

namespace irregular
{

std::tuple< Vect_2, double> overlap( 
    const Polygon_2 &poly1, const Point_2 &emplacement1, const Polygon_2 &poly2, const Point_2 &emplacement2, const Polygon_with_holes_2 &NFP);

Vect_2 orthogonal_projection(
     Point_2 A, Point_2 B, Point_2 M)

double norm(Vect_2 v)
}
}