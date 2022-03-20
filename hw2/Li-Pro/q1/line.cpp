#include <cassert>

#include "line.hpp"

Line::Line (size_t size)
    : m_xvec(size), m_yvec(size)
{}

size_t
Line::size() const
{
    assert( m_xvec.size() == m_yvec.size() );
    
    return m_xvec.size();
}

float const &
Line::x(size_t idx) const
{
    return m_xvec[idx];
}

float &
Line::x(size_t idx)
{
    return m_xvec[idx];
}

float const &
Line::y(size_t idx) const
{
    return m_yvec[idx];
}

float &
Line::y(size_t idx)
{
    return m_yvec[idx];
}
