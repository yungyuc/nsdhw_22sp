#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

class Line
{
public:
    Line() = default;

    Line(Line const & other)
    {
        _lineSize = other.size();
        m_x.resize(_lineSize);
        m_y.resize(_lineSize);
        for (size_t it=0; it<_lineSize; ++it)
        {
            x(it) = other.x(it);
            y(it) = other.y(it);
        }
    }

    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    
    Line(size_t size)
    {
        _lineSize = size;
        m_x.resize(_lineSize);
        m_y.resize(_lineSize);
    }

    ~Line() { 
        this->m_x.clear();  
        this->m_y.clear();
    }
    size_t size() const { return _lineSize; }
    float const & x(size_t it) const { return m_x.at(it); }
    float       & x(size_t it)       { return m_x.at(it); }
    float const & y(size_t it) const { return m_y.at(it); }
    float       & y(size_t it)       { return m_y.at(it); }
private:
    size_t _lineSize;
    std::vector<float> m_x;
    std::vector<float> m_y;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}