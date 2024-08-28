#include "irregular/polygon_convex_hull.hpp"

using namespace packingsolver;
using namespace packingsolver::irregular;

namespace
{

int counter_clockwise(
        const Point& point_1,
        const Point& point_2,
        const Point& point_3)
{
    int area = (point_2.x - point_1.x) * (point_3.y - point_1.y)
        - (point_2.y - point_1.y) * (point_3.x - point_1.x);
    if (area > 0) {
        return -1;
    } else if (area < 0) {
        return 1;
    }
    return 0;
}

LengthDbl square_distance(
        const Point& point_1,
        const Point& point_2)
{
    LengthDbl dx = point_1.x - point_2.x;
    LengthDbl dy = point_1.y - point_2.y;
    return dx * dx + dy * dy;
}

}

Shape irregular::polygon_convex_hull(
        const Shape& shape)
{
    //std::cout << "shape.elements.size() " << shape.elements.size() << std::endl;
    if (shape.elements.size() < 3)
        return shape;

    Point point_least_y = {
        std::numeric_limits<LengthDbl>::infinity(),
        std::numeric_limits<LengthDbl>::infinity()};
    for (const ShapeElement& shape_element: shape.elements) {
        if (point_least_y.y > shape_element.start.y
                || (point_least_y.y == shape_element.start.y
                    && point_least_y.x > shape_element.start.x)) {
            point_least_y = shape_element.start;
        }
    }

    // swap the pivot with the first point
    std::vector<Point> points;
    for (const ShapeElement& shape_element: shape.elements) {
        if (shape_element.start == point_least_y)
            continue;
        points.push_back(shape_element.start);
    }

    std::sort(
            points.begin(),
            points.end(),
            [&point_least_y](
                const Point& point_1,
                const Point& point_2)
            {
                int order = counter_clockwise(
                        point_least_y,
                        point_1,
                        point_2);
                if (order != 0)
                    return (order == -1);
                return square_distance(point_least_y, point_1)
                    < square_distance(point_least_y, point_2);
            });

    std::vector<Point> convex_hull;
    convex_hull.push_back(point_least_y);
    //std::cout << "push " << point_least_y.to_string() << std::endl;
    convex_hull.push_back(points[0]);
    //std::cout << "push " << points[0].to_string() << std::endl;
    convex_hull.push_back(points[1]);
    //std::cout << "push " << points[1].to_string() << std::endl;

    for (int pos = 2; pos < (ElementPos)points.size(); pos++) {
        //std::cout << "pos " << pos << " / " << points.size() << std::endl;
        //std::cout << "point " << points[pos].to_string() << std::endl;
        Point top = convex_hull.back();
        convex_hull.pop_back();
        //std::cout << "pop " << top.to_string() << std::endl;
        while (convex_hull.size() > 1
                && counter_clockwise(convex_hull.back(), top, points[pos]) != -1) {
            top = convex_hull.back();
            convex_hull.pop_back();
            //std::cout << "pop " << top.to_string() << std::endl;
        }
        convex_hull.push_back(top);
        //std::cout << "push " << top.to_string() << std::endl;
        convex_hull.push_back(points[pos]);
        //std::cout << "push " << points[pos].to_string() << std::endl;
    }

    Shape convex_hull_shape;
    for (ElementPos pos = 0;
            pos < convex_hull.size();
            ++pos) {
        ShapeElement element;
        element.type = ShapeElementType::LineSegment;
        element.start = convex_hull[pos];
        element.end = convex_hull[(pos + 1) % convex_hull.size()];
        convex_hull_shape.elements.push_back(element);
    }
    return convex_hull_shape;
}
