#include "packingsolver/irregular/instance.hpp"
#include "irregular/minkowski.hpp"
#include <CGAL/minkowski_sum_2.h>

// typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;

namespace packingsolver
{

namespace irregular
{

std::tuple<double, double, double, double> calculateBounds(
    const Shape &polygon);

Point_2 random_point_in_shape(
    const Shape &polygon );

Point_2* glouton(const Instance &instance);

}
}