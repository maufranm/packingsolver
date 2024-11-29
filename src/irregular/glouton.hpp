#include "packingsolver/irregular/instance.hpp"
#include <CGAL/minkowski_sum_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;

namespace packingsolver
{

namespace irregular
{

std::tuple<double, double, double, double> calculateBounds(
    Shape polygon);

Point_2 random_point_in_shape(
    const Shape &polygon );

Point_2* glouton(const Instance &instance);

}
}