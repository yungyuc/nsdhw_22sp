//
//  point.cpp
//  hw2-q1
//
//  Created by MinChunCho on 2022/3/8.
//

#include "point.hpp"

Point::Point(Point const& obj){
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}

Point & Point::operator=(Point const& obj){
    this->x_ = obj.x_;
    this->y_ = obj.y_;
    return *this;
}

Point::Point(Point && obj){
    std::swap(this->x_, obj.x_);
    std::swap(this->y_, obj.y_);
}

Point & Point::operator=(Point && obj){
    std::swap(this->x_, obj.x_);
    std::swap(this->y_, obj.y_);
    return *this;
}

float Point::x() const{
    return x_;
}

void Point::x(float const& x){
    x_ = x;
}

float Point::y() const{
    return y_;
}

void Point::y(float const& y){
    y_ = y;
}
