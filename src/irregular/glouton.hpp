#include "packingsolver/irregular/instance.hpp"
#include <CGAL/minkowski_sum_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;

namespace packingsolver
{

namespace irregular
{

Point_2 random_point_in_shape(
    Shape polygon );

Point_2* glouton(Instance instance);

}
}