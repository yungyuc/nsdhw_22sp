//
//  point.hpp
//  hw2-q1
//
//  Created by MinChunCho on 2022/3/8.
//

#ifndef point_hpp
#define point_hpp

#include <iostream>
#include <algorithm>
#include <utility>

class Point {
public:
    Point();
    Point(int const&, int const&);
    Point(Point const&);
    Point & operator=(Point const&);
    Point(Point &&);
    Point & operator=(Point &&);
    ~Point();
    
    float x() const;
    void x(float const&);
    float y() const;
    void y(float const&);
private:
    float x_;
    float y_;
};

inline
Point::Point(): x_(0), y_(0){}

inline
Point::Point(int const& x, int const& y): x_(x), y_(y){}

inline
Point::~Point(){}

#endif /* point_hpp */
