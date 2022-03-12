#pragma once

#include <vector>

/*
Container for 2D float points.
Default destructed & copy constructed, thanks to STL containers.
*/
class Line
{
public:
    Line(size_t size);

    size_t size() const;

    float const & x(size_t idx) const;
    float       & x(size_t idx)      ;
    float const & y(size_t idx) const;
    float       & y(size_t idx)      ;

private:
    std::vector<float> m_xvec;
    std::vector<float> m_yvec;
};
