#pragma once
#include <vector>
#include <cstddef>


#include <stdexcept>
class Line
{
public:
  Line() = default;
  Line(Line const & );
  Line(Line       &&);
  Line & operator=(Line const & );
  Line & operator=(Line       &&);
  Line(size_t size);
  ~Line() = default;
  size_t size() const;
  const float & x(size_t it) const;
  float & x(size_t it);
  const float & y(size_t it) const;
  float & y(size_t it);
private:
  void check_range(size_t it) const;
  struct Point { 
    float x, y;
  };
  std::vector<Point> p;
}; 

