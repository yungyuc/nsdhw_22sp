#include "line.hpp"

/*
https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects/255744#255744
https://stackoverflow.com/questions/14977632/copying-stdvector-prefer-assignment-or-stdcopy
*/

// Constructor
Line::Line()
{
    coordinate.resize(0);
}

// Copy constructor
Line::Line(Line const &other)
{
    coordinate = other.coordinate;
}

// Move constructor
Line::Line(Line &&other) : Line()
{
    swap(*this, other);
}

// Copy assignment operator
Line& Line::operator=(Line const &other)
{
    if (this != &other)
    {
        coordinate = other.coordinate;
    }
    return *this;
}

// Move assignment operator
Line& Line::operator=(Line &&other)
{
    if (this != &other)
    {
        swap(*this, other);
    }
    return *this;
}

// Custom constructor
Line::Line(size_t size)
{
    coordinate.resize(size * 2);
}

// Destructor
Line::~Line()
{
    coordinate.clear();         // remove all elements
    coordinate.shrink_to_fit(); // reduces the vector’s capacity to fit the size
}

// Accessor
size_t Line::size() const
{
    return coordinate.size() / 2;
}
float Line::x(size_t it) const
{
    return coordinate.at(it * 2);
}
float& Line::x(size_t it)
{
    return coordinate.at(it * 2);
}
float Line::y(size_t it) const
{
    return coordinate.at(it * 2 + 1);
}
float & Line::y(size_t it)
{
    return coordinate.at(it * 2 + 1);
}