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
typedef BASE::Hole_const_iterator;


using namespace packingsolver;
using namespace packingsolver::irregular;


std::vector< std::pair<Point_2, Point_2> > get_edges(Polygon_with_holes_2 poly)
{
    Polygon_2 contour = poly.outer_boundary();
    //prendre arrêtes

    for( Hole_const_iterator index = poly.holes_begin; index)
    for( int i =0; i<poly.number_of_holes(); i++){



    }


}




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


// renvoie [(0,0);0] si ne s'intersectent pas, sinon déplacement et distance vers l'arrête la plus proche
std::tuple< Vect_2, double> overlap( 
    Polygon_2 poly1, Point_2 emplacement1, Polygon_2 poly2, Point_2 emplacement2,  Polygon_with_holes_2 NFP)
{

    if(is_intersected(poly1, emplacement1, poly2, emplacement2, NFP)==false) 
    {
        std::tuple< Vect_2, double> NoOverlap = { Vect_2(0.0 , 0.0), 0.0 };
        return(NoOverlap);
    }


    Vect_2 translation = orthogonal_projection( *(NFP.vertices_end()), *(NFP.vertices_begin()), *(poly2.vertices_begin()) ) ;
    std::tuple< Vect_2, double > min = {translation, norm(translation)};

    for (ElementPos pos = 0; pos < (ElementPos)NFP.size() - 1; ++pos)
    {

        Vect_2 translation = orthogonal_projection( NFP[pos] , NFP[++pos] , *(poly2.vertices_begin()) );
        double distance = norm(translation);
        if( distance < std::get<1>(min) )
        {
            min = { translation, distance};
        }
    }
    return(min);
}
