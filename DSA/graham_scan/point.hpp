#ifndef POINT_HPP
#define POINT_HPP

#include <climits> 
#include <cmath> 

struct Point {

    double x, y;
    Point() : x(0.), y(0.) {}
    Point(double x, double y) : x(x), y(y) {}

};

double slope(Point* p1, Point* p2) {

    double delta_y = (p2 -> y) - (p1 -> y); 
    double delta_x = (p2 -> x) - (p1 -> x);
    if (delta_x == 0) return INT_MAX;
    return delta_y / delta_x;

}

double distance(Point* p1, Point* p2) {
    double d = sqrt((p1 -> x - p2 -> x)*(p1 -> x - p2 -> x) + (p1 -> y - p2 -> y)*(p1 -> y - p2 -> y));
    return d;
}

struct SortByX {

    constexpr SortByX() {}
    bool operator()(Point* p1, Point* p2) const {
        return (p1 -> x) <= (p2 -> x); 
    }

};

struct SortByY {

    constexpr SortByY() {}
    bool operator()(Point* p1, Point* p2) const {
        return (p1 -> y) <= (p2 -> y); 
    }

};

#endif