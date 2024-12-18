#include "irregular/glouton.hpp"

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
    const Shape &polygon)
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


//dorénavent, cette fonction prend en argument le container ET UN ITEM et renvoie un point tel que le rectangle englobant l'item sera dans le rectangle englobant le container
Point_2 irregular::random_point_in_shape(
    const Shape &container, const Polygon_2 &item )
{
    auto [x_min, x_max, y_min, y_max] = calculateBounds(container);
    LengthDbl x=-1; //neg coord banned
    LengthDbl y=-1;
    Polygon_2 poly = get_poly(container);

    LengthDbl x_diff_right = item.right_vertex()[0][0] - item.vertices_begin()[0][0];
    LengthDbl x_diff_left = - item.left_vertex()[0][0] + item.vertices_begin()[0][0];
    LengthDbl y_diff_up = item.top_vertex()[0][1] - item.vertices_begin()[0][1];
    LengthDbl y_diff_bottom = - item.bottom_vertex()[0][1] + item.vertices_begin()[0][1];


    std::default_random_engine re;
    std::random_device rd;
    re.seed(rd());
    std::default_random_engine re2;
    re2.seed(rd());

    while ( CGAL::oriented_side(Point_2(x,y),poly)== CGAL::ON_NEGATIVE_SIDE )
    {
        std::uniform_real_distribution<double> unif_X(x_min + x_diff_left  ,  x_max - x_diff_right);
        x = unif_X(re);

        std::uniform_real_distribution<double> unif_Y(y_min + y_diff_bottom  ,  y_max - y_diff_up); 
        y = unif_Y(re2);
    }
    return( Point_2(x,y) );
}
//test si p dans rect


/*
 * GLOUTON
 * return: solution, where bl_corner is the placement of the first point for each item
 */
Solution irregular::glouton(const Instance &instance
    /*Shape container, std::vector<Shape> itemsList */)  // itemList supposed ordered by value
{
    int nombreEssaisPlacement = 25;
    ItemTypeId N = instance.number_of_item_types();

    

    /* CONVERSIONS */
    // conversion from Shape "edges" to Polygon_2 "edges"
    Shape container = irregular::clean_shape(instance.bin_type(0).shape);
    std::vector<Shape> bords = borders(container);
    
    std::vector<Polygon_2> container_borders={};

    std::cerr << "NUMBER OF BORDERS: " << bords.size() << std::endl;

       for (int i = 0; i < bords.size(); i++)
    {
        Shape bord_shape = bords[i];
        for (auto el = bord_shape.elements.begin(); el != bord_shape.elements.end(); el++)
        {
            std::cerr << (*el).start.x << ",";
            std::cerr << (*el).start.y << " ";
        }
        std::cerr << std::endl;
    }

    for( int i=0; i<bords.size() ; i++)
    {
        container_borders.push_back(get_poly(bords[i]));
    }



    // conversion of items for NFP
    std::vector<Polygon_2> items_converted_to_polygons={};
    for (ItemTypeId i=0; i<N; i++)
    {
        items_converted_to_polygons.push_back(get_poly(instance.item_type(i).shapes[0].shape));
    }

    // initialise solution instance
    Solution sol(instance);
    sol.add_bin(0,1);

    // vector of indexes
    std::vector<ItemTypeId> placed_item_ids={};
    

    // Point_2 item_positions[N];
    // // initialise positions ( (-1,-1) at the end means that the object isn't placed)
    // for (ItemTypeId i = 0; i < N; i++)
    // {
    //     item_positions[i] = Point_2(-1,-1);
    // }
    

    /* PLACING ITH ITEM */
    for (ItemTypeId i=0; i<N; i++)
    {
        int n=placed_item_ids.size();
        Polygon_2* placing_polygon = &items_converted_to_polygons[i];

        // calculs des NFP avec ce polygone
        // TODO: remplacer par hash table
        std::vector<Polygon_with_holes_2> NFPsList_borders = {};
        for(int index_border=0; index_border< container_borders.size(); index_border++)
        {
            NFPsList_borders.push_back(NFP(container_borders[index_border], *placing_polygon));
        }
        std::vector<Polygon_with_holes_2> NFPsList = {};
        for (int j=0; j<n; j++)
        {
            NFPsList.push_back(NFP(items_converted_to_polygons[j] , *placing_polygon));
        }
        


        for (int k=0; k< nombreEssaisPlacement; k++)
        {
            
            Point_2 position = random_point_in_shape(container, *placing_polygon);




            bool is_feasible_position = true;
            // check if current placing_polygon is totally inside the container
            // NB: only the reference point is in the container as far as we know
            for(int index_border=0; index_border< NFPsList_borders.size(); index_border++) 
            {
                if (
                    is_intersected(
                        *placing_polygon,
                        position,
                        container_borders[index_border],
                        *container_borders[index_border].vertices_begin(),
                        NFPsList_borders[index_border])
                )
                {
                        is_feasible_position = false;
                        break;
                }
            }

            if (is_feasible_position) {
            
            //check if current placing_polygon intersects any of the previously placed polygons
            for(int index_placed=0; index_placed<n; index_placed++) 
            {
                //NB: bl_corner is not a corner but a position for us
                Point point = sol.bin(0).items[index_placed].bl_corner;
                if (
                    is_intersected(
                        *placing_polygon,
                        position,
                        items_converted_to_polygons[placed_item_ids[index_placed]],
                        Point_2(point.x,point.y),
                        NFPsList[index_placed])
                )
                {
                        is_feasible_position = false;
                        break;
                }
            }
            }

            //if a correct position is found, save it and test next polygon
            if(is_feasible_position)
                {
                    placed_item_ids.push_back(i);
                    sol.add_item( 0, i, {position[0],position[1]}, 0, false);
                    std::cerr << "PLACÉ en :" << position << std::endl;
                    break;
                }
            // otherwise, try another position with the same polygon nombreEssaisPlacement times
            std::cerr << "réessaie - ";
        }
    }
    return sol;
}