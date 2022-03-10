#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & srcLine) : m_x(srcLine.m_x), m_y(srcLine.m_y) {}; // copy constructor.
    Line(Line       && srcLine) : m_x(std::move(srcLine.m_x)), m_y(std::move(srcLine.m_y)) {}; // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Custom constructor.
    Line(size_t size) : m_x(size, 0), m_y(size, 0) {};
    // Desctructor.
    ~Line() {};
    // Accessors.
    size_t size() const { return m_x.size(); }
    float const & x(size_t it) const { return m_x.at(it); }
    float       & x(size_t it)       { return m_x.at(it); }
    float const & y(size_t it) const { return m_y.at(it); }
    float       & y(size_t it)       { return m_y.at(it); }
private:
    std::vector<float> m_x;
    std::vector<float> m_y;
}; /* end class Line */

Line & Line::operator=(Line const & srcLine)
{
    m_x.assign(srcLine.m_x.begin(), srcLine.m_x.end());
    m_y.assign(srcLine.m_y.begin(), srcLine.m_y.end());
    return *this;
};

Line & Line::operator=(Line       && srcLine)
{
    if (this == &srcLine)
        return *this;
    m_x = std::move(srcLine.m_x);
    m_y = std::move(srcLine.m_y);
    return *this;
};

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