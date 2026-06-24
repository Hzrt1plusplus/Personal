#include "./hull.hpp"
#include <iostream> 

int main() {
    std::vector<Point*> points = {
        
        new Point(3, 8), 
        new Point(3, 10), 
        new Point(2, 13),
        new Point(4, 12), 
        new Point(3, 13), 
        new Point(3, 14), 
        new Point(5, 15), 
        new Point(5, 9), 
        new Point(7, 10), 
        new Point(6, 13), 
        new Point(8, 14), 
        new Point(7, 7), 
        new Point(8,9), 
        new Point(8, 12), 
        new Point(10, 13), 
        new Point(12,15), 
        new Point(11,9), 
        new Point(10, 8), 
        new Point(9, 11),     

    };

    auto result = convex_hull(points); 
    std::cout << "Convex Hull: ";
    for (Point* p : result)
        std::cout << "(" << p -> x << "," << p -> y << ") ";
    std::cout << "\n"; 

    for (auto p : points) delete p;
    return 0;
}