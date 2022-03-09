//
//  line.cpp
//  hw2-q1
//
//  Created by MinChunCho on 2022/3/7.
//

#include "line.hpp"

Line::Line(Line const &obj){
    for(auto const& p:obj.points_){
        this->points_.push_back(Point(p));
    }
}

Line & Line::operator=(Line const &obj){
    this->points_.clear();
    for(auto const& p:obj.points_){
        this->points_.push_back(Point(p));
    }
    
    return *this;
}

Line::Line(Line &&obj){
    this->points_ = std::move(obj.points_);
}

Line & Line::operator=(Line &&obj){
    if(this != &obj){
        this->points_ = std::move(obj.points_);
    }
    return *this;
}

Point & Line::operator[](size_t const &idx){
    return points_[idx];
}

size_t Line::size() const{
    return points_.size();
}
