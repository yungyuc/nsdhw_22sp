#include "Line.hpp"
/* Define the Custom constructor*/
Line::Line(size_t size):l(size), m_size(size){};
/* Define the copy constructor */
Line::Line(Line const & other):l(other.l), m_size(other.m_size){}

/* Define the move constructor */
Line::Line(Line && other):l(std::move(other.l)), m_size(other.m_size){}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other){
    if(this != &other){
        l = other.l;
        m_size = other.m_size;
    }
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other){
    if(this != &other){
        l = std::move(other.l);
        m_size = other.m_size;
    }
    return *this;
}