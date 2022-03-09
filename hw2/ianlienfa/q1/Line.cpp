#include "line.h"

Line::Line(){}

// Copy constructor
Line::Line(Line const & line)
{
    x_ = line.x_;
    y_ = line.y_;
    size_ = line.size_;
}

// Move constructor
Line::Line(Line &&line)
{
    x_ = std::move(line.x_);
    y_ = std::move(line.y_);
    size_ = std::move(line.size_);
}

Line& Line::operator=(Line const & line)
{
    if(line.size() != 0)
    {
        size_ = line.size_;
        x_ = line.x_;
        y_ = line.y_;
    }
    return *this;
}

Line& Line::operator=(Line && line)
{
    if(line.size() != 0)
    {
        size_ = std::move(line.size_);
        x_ = std::move(line.x_);
        y_ = std::move(line.y_);
    }
    return *this;
}

Line::Line(size_t size)
{
    x_.resize(size);
    y_.resize(size);
    this->size_ = size;
}

// since we use STL containers, we don't quite need to define the destructor
Line::~Line(){}

size_t Line::size() const
{
    return size_;
}

const float& Line::x(size_t it) const
{
    if (it >= size_) { throw std::out_of_range("Line index out of range"); }
    return this->x_[it];   
}
float& Line::x(size_t it)
{
    if (it >= size_) { throw std::out_of_range("Line index out of range"); }
    return x_[it];
}
const float& Line::y(size_t it) const
{
    if (it >= size_) { throw std::out_of_range("Line index out of range"); }
    return y_[it];   
}
float& Line::y(size_t it)
{
    if (it >= size_) { throw std::out_of_range("Line index out of range"); }
    return y_[it];
}
