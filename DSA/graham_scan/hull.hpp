#ifndef HULL_HPP
#define HULL_HPP

#include "sort.hpp"
#include "point.hpp"
#include <memory> 

/*
    Debug
*/
#include <iostream> 

using std::shared_ptr;
using std::make_shared;

int ccw(Point* prev, Point* current, Point* next) 
{
    double v1 = ((current -> y - prev -> y)*(next -> x - current -> x));
    double v2 = ((current -> x - prev -> x)*(next -> y - current -> y)); 

    int val = v1 - v2;
    
    if (val < 0) return 1;
    else if (val > 0) return -1;

    return 0;
}

struct SlopeRef {

    Point* _p;
    double _slope;

    SlopeRef(Point* p, Point* ref_p) : _p(p) {
        _slope = slope(p, ref_p); 
    }

    SlopeRef& operator=(const SlopeRef& o) {
        _p = o._p;
        _slope = o._slope;
        return *this;
    }

    struct CompareBySlope {

        using ptr = std::shared_ptr<SlopeRef>; 

        constexpr CompareBySlope() {}
        bool operator()(ptr r1, ptr r2) const {
            return (r1 -> _slope) <= (r2 -> _slope); 
        }

    };

};

struct PointRef {

    Point *p, *P0;

    PointRef(Point* p, Point* P0) : p(p), P0(P0) {}

    struct PointRefComparator {

        using ptr = std::shared_ptr<PointRef>; 
        constexpr PointRefComparator() {}
        bool operator()(ptr p1, ptr p2) const {
            int c = ccw(p1 -> P0, p1 -> p, p2 -> p);
            if (c != 0)
                return c > 0;
            return distance(p1 -> p, p1 -> P0) <= distance(p2 -> p, p2 -> P0);  
        }

    };

};

Point* find_ref_point(std::vector<Point*>& v) { // O(n)
    Point* ref = v[0]; 
    for (int i = 1; i < v.size(); i++) {
        if (v[i] -> y < ref -> y) {
            ref = v[i]; 
        } else if (v[i] -> y == ref -> y) {
            if (v[i] -> x < ref -> x)
                ref = v[i]; 
        }
    } 
    return ref;
}


std::vector<Point*> convex_hull(std::vector<Point*>& points) {

    std::vector<Point*> convexHull;
    const int size = points.size(); 
    if (size < 3) return convexHull;

    Point* P0 = find_ref_point(points); 

    std::vector<std::shared_ptr<PointRef>> slopes;
    for (auto p : points)
        slopes.push_back(std::make_shared<PointRef>(p, P0)); 

    MergeSort<std::shared_ptr<PointRef>, PointRef::PointRefComparator>::sort(slopes); 
    int hullSize = 0;

    for (int i = 0; i < size; i++) {
        while (hullSize > 1 && ccw(convexHull[hullSize - 2], convexHull[hullSize - 1], slopes[i] -> p) <= 0) {
            convexHull.pop_back(); 
            hullSize--;
        }
        convexHull.push_back(slopes[i] -> p); 
        hullSize++;
    }

    return convexHull;

}   

#endif 