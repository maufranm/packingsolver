#include "irregular/shape.hpp"

using namespace packingsolver;
using namespace packingsolver::irregular;

std::pair<bool, Shape> irregular::remove_redundant_vertices(
        const Shape& shape)
{
    bool found = false;
    Shape shape_new;

    for (ElementPos element_pos = 0;
            element_pos < (ElementPos)shape.elements.size();
            ++element_pos) {
        const ShapeElement& element = shape.elements[element_pos];
        if (equal(element.start.x, element.end.x)
                && equal(element.start.y, element.end.y)) {
            found = true;
            continue;
        }
        shape_new.elements.push_back(element);
    }

    return {found, shape_new};
}

std::pair<bool, Shape> irregular::remove_aligned_vertices(
        const Shape& shape)
{
    if (shape.elements.size() <= 3)
        return {false, shape};

    bool found = false;
    Shape shape_new;

    ElementPos element_prev_pos = shape.elements.size() - 1;
    for (ElementPos element_cur_pos = 0;
            element_cur_pos < (ElementPos)shape.elements.size();
            ++element_cur_pos) {
        ElementPos element_next_pos = element_cur_pos + 1;
        const ShapeElement& element_prev = shape.elements[element_prev_pos];
        const ShapeElement& element = shape.elements[element_cur_pos];
        const ShapeElement& element_next = (element_next_pos < shape.elements.size())?
            shape.elements[element_next_pos]:
            shape_new.elements.front();
        bool useless = false;
        if (element.type == ShapeElementType::LineSegment
                && element_prev.type == ShapeElementType::LineSegment) {
            double x1 = element_prev.start.x;
            double y1 = element_prev.start.y;
            double x2 = element.start.x;
            double y2 = element.start.y;
            double x3 = element_next.start.x;
            double y3 = element_next.start.y;
            double v = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
            //std::cout << "element_prev  " << element_prev_pos << " " << element_prev.to_string() << std::endl;
            //std::cout << "element       " << element_cur_pos << " " << element.to_string() << std::endl;
            //std::cout << "element_next  " << element_next_pos << " " << element_next.to_string() << std::endl;
            //std::cout << "v " << v << std::endl;
            if (equal(v, 0)
                    || (equal(element_prev.start.y, element.start.y)
                        && equal(element.start.y, element_next.start.y))
                    || (equal(element_prev.start.x, element.start.x)
                        && equal(element.start.x, element_next.start.x))) {
                //std::cout << "useless " << element.to_string() << std::endl;
                useless = true;
                found = true;
            }
        }
        if (!useless) {
            if (!shape_new.elements.empty())
                shape_new.elements.back().end = element.start;
            shape_new.elements.push_back(element);
            element_prev_pos = element_cur_pos;
        }
    }
    shape_new.elements.back().end = shape_new.elements.front().start;

    return {found, shape_new};
}

std::pair<bool, Shape> irregular::equalize_close_y(
        const Shape& shape)
{
    bool found = false;
    Shape shape_new;

    ElementPos element_prev_pos = shape.elements.size() - 1;
    for (ElementPos element_pos = 0;
            element_pos < (ElementPos)shape.elements.size();
            ++element_pos) {
        const ShapeElement& element = shape.elements[element_pos];
        const ShapeElement& element_prev = shape.elements[element_prev_pos];
        shape_new.elements.push_back(element);
        if (equal(element.start.y, element_prev.start.y)
                && element.start.y != element_prev.start.y) {
            shape_new.elements.back().start.y = element_prev.start.y;
            found = true;
        }
        element_prev_pos = element_pos;
    }

    return {found, shape_new};
}

Shape irregular::clean_shape(
        const Shape& shape)
{
    Shape shape_new = shape;

    auto res = remove_redundant_vertices(shape_new);
    shape_new = res.second;

    for (;;) {
        bool found = false;

        {
            auto res = remove_aligned_vertices(shape_new);
            found |= res.first;
            shape_new = res.second;
        }

        {
            auto res = equalize_close_y(shape_new);
            found |= res.first;
            shape_new = res.second;
        }

        if (!found)
            break;
    }

    return shape_new;
}

std::vector<Shape> packingsolver::irregular::borders(
        const Shape& shape)
{
    std::vector<Shape> res;

    auto mm = shape.compute_min_max();

    Shape shape_border;
    ElementPos element_0_pos = 0;
    for (ElementPos element_pos = 0;
            element_pos < shape.elements.size();
            ++element_pos) {
        const ShapeElement& shape_element = shape.elements[element_pos];
        if (shape_element.start.x == mm.first.x) {
            element_0_pos = element_pos;
            break;
        }
    }
    // 0: left; 1: bottom; 2: right; 3: top.
    const ShapeElement& element_0 = shape.elements[element_0_pos];
    int start_border = (element_0.start.y == mm.first.y)? 1: 0;
    LengthDbl start_coordinate = element_0.start.y;
    for (ElementPos element_pos = 0;
            element_pos < shape.elements.size();
            ++element_pos) {
        const ShapeElement& element = shape.elements[(element_0_pos + element_pos) % shape.elements.size()];
        //std::cout << "element_pos " << ((element_0_pos + element_pos) % bin_type.shape.elements.size()) << " / " << bin_type.shape.elements.size() << ": " << element.to_string() << std::endl;
        shape_border.elements.push_back(element);
        bool close = false;
        if (start_border == 0) {
            if (equal(element.end.x, mm.first.x)) {
                ShapeElement new_element;
                new_element.type = ShapeElementType::LineSegment;
                new_element.start = element.end;
                new_element.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element);
                close = true;
                start_border = 0;
            } else if (equal(element.end.y, mm.first.y)) {
                ShapeElement new_element_1;
                new_element_1.type = ShapeElementType::LineSegment;
                new_element_1.start = element.end;
                new_element_1.end = {mm.first.x, mm.first.y};
                shape_border.elements.push_back(new_element_1);
                ShapeElement new_element_2;
                new_element_2.type = ShapeElementType::LineSegment;
                new_element_2.start = new_element_1.end;
                new_element_2.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element_2);
                close = true;
                start_border = 1;
            }
        } else if (start_border == 1) {
            if (equal(element.end.y, mm.first.y)) {
                ShapeElement new_element;
                new_element.type = ShapeElementType::LineSegment;
                new_element.start = element.end;
                new_element.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element);
                close = true;
                start_border = 1;
            } else if (equal(element.end.x, mm.second.x)) {
                ShapeElement new_element_1;
                new_element_1.type = ShapeElementType::LineSegment;
                new_element_1.start = element.end;
                new_element_1.end = {mm.second.x, mm.first.y};
                shape_border.elements.push_back(new_element_1);
                ShapeElement new_element_2;
                new_element_2.type = ShapeElementType::LineSegment;
                new_element_2.start = new_element_1.end;
                new_element_2.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element_2);
                close = true;
                start_border = 2;
            }
        } else if (start_border == 2) {
            if (equal(element.end.x, mm.second.x)) {
                ShapeElement new_element;
                new_element.type = ShapeElementType::LineSegment;
                new_element.start = element.end;
                new_element.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element);
                close = true;
                start_border = 2;
            } else if (equal(element.end.y, mm.second.y)) {
                ShapeElement new_element_1;
                new_element_1.type = ShapeElementType::LineSegment;
                new_element_1.start = element.end;
                new_element_1.end = {mm.second.x, mm.second.y};
                shape_border.elements.push_back(new_element_1);
                ShapeElement new_element_2;
                new_element_2.type = ShapeElementType::LineSegment;
                new_element_2.start = new_element_1.end;
                new_element_2.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element_2);
                close = true;
                start_border = 3;
            }
        } else if (start_border == 3) {
            if (equal(element.end.y, mm.second.y)) {
                ShapeElement new_element;
                new_element.type = ShapeElementType::LineSegment;
                new_element.start = element.end;
                new_element.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element);
                close = true;
                start_border = 3;
            } else if (equal(element.end.x, mm.first.x)) {
                ShapeElement new_element_1;
                new_element_1.type = ShapeElementType::LineSegment;
                new_element_1.start = element.end;
                new_element_1.end = {mm.first.x, mm.second.y};
                shape_border.elements.push_back(new_element_1);
                ShapeElement new_element_2;
                new_element_2.type = ShapeElementType::LineSegment;
                new_element_2.start = new_element_1.end;
                new_element_2.end = shape_border.elements[0].start;
                shape_border.elements.push_back(new_element_2);
                close = true;
                start_border = 0;
            }
        }
        // New shape.
        if (close) {
            if (shape_border.elements.size() >= 3)
                res.push_back(shape_border.reverse());
            shape_border.elements.clear();
        }
    }

    return res;
}
