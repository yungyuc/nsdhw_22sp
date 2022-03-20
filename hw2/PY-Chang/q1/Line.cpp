//
//  Line.cpp
//  nsd_hw2
//
//  Created by 張秉洋 on 2022/3/9.
//

#include <iostream>
#include "Line.hpp"

// Custom constructor.
Line::Line(size_t size) { this->m_coord.reserve(size);}

// copy constructor.
Line::Line(Line const & toCopy){
    size_t size = toCopy.m_coord.capacity();
    this->m_coord.reserve(size);
    for(size_t i=0; i<size; i++){
        this->m_coord[i].x = toCopy.x(i);
        this->m_coord[i].y = toCopy.y(i);
    }
}

// move constructor.
Line::Line(Line && toMove){
    std::swap(toMove.m_coord, this->m_coord);
}

// copy assignment operator.
Line & Line::operator=(Line const & toCopy)
{
    if (this == &toCopy) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    size_t size = toCopy.m_coord.capacity();
    this->m_coord.reserve(size);
    for(size_t i=0; i<size; i++){
        this->m_coord[i].x = toCopy.x(i);
        this->m_coord[i].y = toCopy.y(i);
    }
    return *this;
}

// move assignment operator.
Line & Line::operator=(Line && toMove)
{
    if (this == &toMove) { return *this; } // don't move to self.
    std::swap(toMove.m_coord, this->m_coord);
    return *this;
}

// Desctructor
Line::~Line() {this->m_coord.clear();}

size_t Line::size() const { return this->m_coord.capacity(); }

const int & Line::x(size_t index) const {
    check_range(index);
    return this->m_coord[index].x;
}

int & Line::x(size_t index) {
    check_range(index);
    return this->m_coord[index].x;
}

const int & Line::y(size_t index) const {
    check_range(index);
    return this->m_coord[index].y;
}
int & Line::y(size_t index) {
    check_range(index);
    return this->m_coord[index].y;
}

void Line::check_range(size_t it) const{
    if (it >= this->m_coord.capacity()) { throw std::out_of_range("Line index out of range"); }
}