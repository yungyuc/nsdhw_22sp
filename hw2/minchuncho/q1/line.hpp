//
//  line.hpp
//  hw2-q1
//
//  Created by MinChunCho on 2022/3/7.
//

#ifndef line_hpp
#define line_hpp

#include "point.hpp"

#include <vector>
#include <iostream>

class Line {
public:
    // constructors
    Line();
    Line(size_t size);
    // copy constructor
    Line(Line const &);
    // copy assignment
    Line & operator=(Line const &);
    // move constructor
    Line(Line &&);
    // move assignment
    Line & operator=(Line &&);
    // operator overloading
    Point & operator[](size_t const &);
    //destructor
    ~Line();
    // member functions
    size_t size() const;
private:
    std::vector<Point> points_;
};

inline
Line::Line(){}

inline
Line::Line(size_t size){
    points_.resize(size, Point(0, 0));
}

inline
Line::~Line(){}

#endif /* line_hpp */
