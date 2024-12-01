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

using namespace packingsolver;
using namespace packingsolver::irregular;


double norm(Vect_2 v)
{
    double n = sqrt(v[0]*v[0] + v[1]*v[1]);
    return n;
}

// si H est le projeté orthogonal de M sur (AB), renvoie le vecteur MH
Vect_2 orthogonal_projection(
    Point_2 A, Point_2 B, Point_2 M)
{
    Vect_2 v= B-A;
    v= v/norm(v);
    Vect_2 MA = A-M;
    Vect_2 MH = MA - CGAL::scalar_product(MA, v)*v;
    
    return(MH);
}

std::tuple< Vect_2, Point_2> overlap( 
    Polygon_2 poly1, Point_2 emplacement1, Polygon_2 poly2, Point_2 emplacement2,  Polygon_with_holes_2 NFP)
{
    if(is_intersected(poly1, emplacement1, poly2, emplacement2, NFP)==false) 
    {
        std::tuple< Vect_2, Point_2> NoOverlap = { Vect_2(0.0 , 0.0), Point_2(0.0 , 0.0)  };
        return(NoOverlap);
    }

    Vect_2 translation = orthogonal_projection( *(poly1.vertices_end()), *(poly1.vertices_begin(), *(poly2.vertices_begin())) ;

    std::tuple< Vect_2 max = {truc};
    for(// parcours arrêtes poly1...)
    {
        edge= ;
        Vect_2 v = orthogonal_projection(v1,v2, *(poly2.vertices_begin()) );
        double norm = norm(v);
        if(norm>max[1])
        {
            max = { v, norm(v)};
        }
    }
    return(max);


}
