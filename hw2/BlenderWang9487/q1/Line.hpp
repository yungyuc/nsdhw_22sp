#pragma once
#include <vector>
#include <iostream>

class Line
{
  private:
    std::vector<float> _x;
    std::vector<float> _y;
  public:
    Line(){}
    Line(Line const &  l):_x(l._x),_y(l._y){}
    Line(Line       && l):
        _x(std::forward<Line>(l)._x),
        _y(std::forward<Line>(l)._y){}
    Line & operator=(Line const &  l)
    {
        _x = l._x;
        _y = l._y;
        return *this;
    }
    Line & operator=(Line       && l)
    {
        _x = std::forward<Line>(l)._x;
        _y = std::forward<Line>(l)._y;
        return *this;
    }
    Line(size_t size):_x(size),_y(size){}
    ~Line(){}
    size_t size() const
    {
        return _x.size();
    }
    const float & x(size_t it) const
    {
        return _x.at(it);
    }
    float & x(size_t it)
    {
        return _x.at(it);
    }
    const float & y(size_t it) const
    {
        return _y.at(it);
    }
    float & y(size_t it)
    {
        return _y.at(it);
    }
};