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

std::vector<Shape> glouton(
    Shape container, std::vector<Shape> itemsList )  // itemList supposed ordered by value
{
    int nombreEssaisPlacement = 10;
    int N = itemsList.size();
    std::vector<Shape> solution;

    for (int i=0; i<N; i++)    // on essaie de placer le i-ème polygone de la liste
    {
        int n=solution.size();
        Shape polygon = itemsList[i];

        // calculs des NFP avec ce polygone
        std::vector<Shape> NFPsList[n];
        for (int j=0; j<n; j++)
        {
            NFPsList[j] = irregular::NFP( solution[j], polygon);
        }


        for (int k=0; k< nombreEssaisPlacement; k++)
        {
            Point_2 position = /* random point in IFP(container, itemlist[i])*/;
            if(
                for (int j=0; j<n; j++){
                    is_in(position, NFPsList[j])== False;   //is_in à faire???
                })
                {
                    //on place le premier sommet du polygone à l'emplacement trouvé
                    polygon.shift(position[0] - get_point(polygon.elements[0])[0], position[1] - get_point(polygon.elements[0])[1] );
                    solution.insert(solution.begin(), polygon);
                    break;
                }
            //sinon, on réessaye un autre placement, puis passe au polygone suivant après nombreEssaisPlacement
        }
    }

    return solution;
} 