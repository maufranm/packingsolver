#include "packingsolver/irregular/instance.hpp"
#include "irregular/minkowski.hpp"


using namespace packingsolver;
using namespace packingsolver::irregular;


/*

def order_items(instance problème):
    prend les items, calcul leur valeurs vi:= ci/aire  (ci dans instance, shape.compute_area() pour aire)
    réordonne les items par valeur vi décroissante
    retourne une liste de polygones ordonnée par priorité de placement

def first_try(instance problème):
    list_items=oder_items(instance problème)          //liste de shapes
    list_placed_poly=[]                               //liste polygones placés dans l'espace

    for item in list_items{

        
        list_NFP=[]                                   //liste de Polygon_with_holes_2
        for placed_item in list_placed_poly{
            list_NFP.append( NFP(placed_item,item) )
        }

        for( int i=0; i < 10; i++){                          //on leur laisse dix chances, ou tout autre valeur arbitraire
            take (x,y) in IFP(container, item) at random     //container à récupérer dans l'instance, random avec ???
            if(
                for NFP in list_NFP{
                    (x,y) is not in NFP                      // fonction is_in...
                }
            )
            then {
                list_placed_poly.append( item +(x,y)-item[0] )  
                break for
            }
        }
    }

    [générer instance solution avec list_placed_poly]

}
*/