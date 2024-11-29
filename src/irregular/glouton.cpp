#include "packingsolver/irregular/instance.hpp"
#include "irregular/glouton.hpp"
#include "irregular/minkowski.hpp"
// #include "irregular/solution.hpp" // TODO: replace list solution with "Solution" class

#include <random>

using namespace packingsolver;
using namespace packingsolver::irregular;


/*
def order_items(instance problème):
    prend les items, calcul leur valeurs vi:= ci/aire  (ci dans instance, shape.compute_area() pour aire)
    réordonne les items par valeur vi décroissante
    retourne une liste de polygones ordonnée par priorité de placement
*/

std::tuple<double, double, double, double> irregular::calculateBounds(
    Shape polygon)
{
    LengthDbl x_min = get_point(polygon.elements[0])[0];
    LengthDbl x_max = x_min;
    LengthDbl y_min = get_point(polygon.elements[0])[1];
    LengthDbl y_max = x_max;

    for(int i=1; i<polygon.elements.size(); i++)
    {
        double x = get_point(polygon.elements[i])[0];
        double y = get_point(polygon.elements[i])[1];
        if (x < x_min){
            x_min = x;
        }
        if (x > x_max){
            x_max = x;
        }
        if (y < y_min){
            y_min = y;
        }
        if (y > y_max){
            y_max = y;
        }
    }
    
    return {x_min, x_max, y_min, y_max};
}

Point_2 irregular::random_point_in_shape(
    Shape polygon )
{
    auto [x_min, x_max, y_min, y_max] = calculateBounds(polygon);
    LengthDbl x=-1; //neg coord banned
    LengthDbl y=-1;
    Polygon_2 poly = get_poly(polygon);

    std::default_random_engine re;
    std::random_device rd;
    re.seed(rd());
    std::default_random_engine re2;
    re2.seed(rd());

    while ( CGAL::oriented_side(Point_2(x,y),poly)== CGAL::ON_NEGATIVE_SIDE )
    {
        std::uniform_real_distribution<double> unif_X(x_min, x_max);
        x = unif_X(re);

        std::uniform_real_distribution<double> unif_Y(y_min, y_max); 
        y = unif_Y(re2);
    }
    return( Point_2(x,y) );
}
//test si p dans rect

Point_2* irregular::glouton(const Instance &instance
    /*Shape container, std::vector<Shape> itemsList */)  // itemList supposed ordered by value
{
    std::cerr << "- Entering glouton -" << std::endl;
    int nombreEssaisPlacement = 10;
    ItemTypeId N = instance.number_of_item_types();
    // vector of indexes
    std::vector<ItemTypeId> placed_items={};
    
    // TODO: replace with vector to make sure memory is ok
    Point_2 item_positions[N];
    // initialise positions ( (-1,-1) at the end means that the object isn't placed)
    int ifor = 1;
    std::cerr << "Entering for " << ifor << std::endl; ifor++;
    for (ItemTypeId i = 0; i < N; i++)
    {
        item_positions[i] = Point_2(-1,-1);
    }
    
    // std::vector<Point_2> solution_placement;
    std::vector<Polygon_2> items_converted_to_polygons={};


    std::cerr << "Entering for " << ifor << std::endl; ifor++;
    for (ItemTypeId i=0; i<N; i++)    // all my homies hate shapes
    {
        std::cerr << "Getting " << i << std::endl;
        items_converted_to_polygons.push_back(get_poly(instance.item_type(i).shapes[0].shape));
    }

    std::cerr << "Entering for " << ifor << std::endl; ifor++;
    for (ItemTypeId i=0; i<N; i++)    // on essaie de placer le i-ème polygone de la liste
    {
        int n=placed_items.size();
        Polygon_2 placing_polygon = items_converted_to_polygons[i];

        // calculs des NFP avec ce polygone
        // TODO: remplacer par hash table
        std::vector<Polygon_with_holes_2> NFPsList = {};
        std::cerr << "Entering for " << ifor << std::endl; ifor++;
        for (int j=0; j<n; j++)
        {
            NFPsList.push_back(NFP(items_converted_to_polygons[j] , placing_polygon));
        }


        std::cerr << "Entering for " << ifor << std::endl; ifor++;
        for (int k=0; k< nombreEssaisPlacement; k++)
        {
            
            Point_2 position = random_point_in_shape(instance.bin_type(0).shape);




            bool is_feasible_position = true;
            //check if current placing_polygon intersects any of the previously placed polygons
            std::cerr << "Entering for " << ifor << std::endl; ifor++;
            for(int index_placed=0; index_placed<n; index_placed++)
            {
                if (
                    is_intersected(
                        placing_polygon,
                        position,
                        items_converted_to_polygons[placed_items[index_placed]],
                        item_positions[placed_items[index_placed]],
                        NFPsList[index_placed])
                )
                {
                        is_feasible_position = false;
                        break;
                }
            }

            //if a correct position is found, save it and test next polygon
            if(is_feasible_position)
                {
                    placed_items.push_back(i);
                    item_positions[i]=position;
                    std::cerr << "Position i: " << position << std::endl;
                    break;
                }
            // otherwise, try another position with the same polygon nombreEssaisPlacement times
        }
    }
    return item_positions;
}