#include <stdexcept>
#include <tuple>
#include <cmath>

#include "irregular/gradient.hpp"
#include "packingsolver/irregular/instance.hpp"
#include "irregular/minkowski.hpp"
#include "irregular/gradient.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Vector_2<CGAL::Epick> Vect_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Polygon_with_holes_2::Hole_const_iterator HoleIterator;



using namespace packingsolver;
using namespace packingsolver::irregular;


std::vector< std::pair<Point_2, Point_2> > irregular::get_edges_polygon(
    const Polygon_2& poly)
{
    std::vector< std::pair<Point_2, Point_2>> edges;
    for (auto pos = poly.vertices_begin(); pos != poly.vertices_end(); ++pos)
    {
        auto next = std::next(pos); // Iterator to the next vertex
        if (next == poly.vertices_end()) {
            next = poly.vertices_begin(); // Wrap around to the first vertex if at the end
        }
        std::pair<Point_2, Point_2> edge = { *pos, *next };
        edges.push_back(edge);
    }
    return edges;
}
// renvoie les arrêtes (=couple de points) d'un NFP (bord extérieur + trous)
std::vector< std::pair<Point_2, Point_2>> irregular::get_edges_NFP(
    Polygon_with_holes_2& poly)
{
    Polygon_2 contour = poly.outer_boundary();
    std::vector< std::pair<Point_2, Point_2> > edges = get_edges_polygon(contour);

    for (HoleIterator it=poly.holes_begin(); it!=poly.holes_end(); ++it)
    {
        Polygon_2 hole = *it;
        std::vector< std::pair<Point_2, Point_2> > hole_edge = get_edges_polygon(hole);
        edges.insert(edges.end(),hole_edge.begin(), hole_edge.end());        
    }
    return edges;
}


double irregular::norm(Vect_2 v)
{
    double n = sqrt(v[0]*v[0] + v[1]*v[1]);
    return n;
}

// si H est le projeté orthogonal de M sur (AB), renvoie le vecteur MH
Vect_2 irregular::orthogonal_projection(
    Point_2 A, Point_2 B, Point_2 M)
{
    Vect_2 v= B-A;
    v= v/norm(v);
    Vect_2 MA = A-M;
    Vect_2 MH = MA - CGAL::scalar_product(MA, v)*v;
    
    return(MH);
}


// renvoie [(0,0);0] si ne s'intersectent pas, sinon vecteur de déplacement (et distance) minimal pour que le premier polygone n'intersecte plus le deuxième
std::tuple< Vect_2, double> irregular::overlap( 
    Polygon_2 poly1, Point_2 emplacement1, Polygon_2 poly2, Point_2 emplacement2,  Polygon_with_holes_2 NFP)
{
    if(is_intersected(poly1, emplacement1, poly2, emplacement2, NFP)==false) 
    {
        std::tuple< Vect_2, double> NoOverlap = { Vect_2(0.0 , 0.0), 0.0 };
        return(NoOverlap);
    }


    std::vector< std::pair<Point_2, Point_2>> edges = get_edges_NFP(NFP);

    std::tuple< Vect_2, LengthDbl> min = { Vect_2(0.0 , 0.0), -1.0 };
    for (int i=0; i<edges.size(); i++)
    {
        Point_2 abstract_point = Point_2(0,0) + (emplacement1 - *(poly1.vertices_begin())) - ( emplacement2 -*(poly2.vertices_begin()) );
        Vect_2 translation = orthogonal_projection( edges[i].first , edges[i].second , abstract_point );
        LengthDbl distance = norm(translation);
        if( distance < std::get<1>(min) )
        {
            min = { translation, distance};
        }
    }
    return(min);

/*
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
*/
}