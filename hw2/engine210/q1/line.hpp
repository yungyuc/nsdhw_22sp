#include <stdlib.h>
#include <vector>
#include <utility>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    const float & x(size_t it) const;
    float & x(size_t it);
    const float & y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
	std::vector<std::pair<float,float>> points;
}; /* end class Line */

Line::Line() {
#ifdef debug
    std::cout << "Constructor() is called" << std::endl;
#endif
};

Line::Line(Line const & rhs) : points(rhs.points) {
#ifdef debug
    std::cout << "Copy constructor is called" << std::endl;
#endif
}

Line::Line(Line && rhs) : points(std::move(rhs.points)) {
#ifdef debug
    std::cout << "Move constructor is called" << std::endl;
#endif
}

Line::Line(size_t size) : points(size, {0, 0}) {
#ifdef debug
    std::cout << "Constructor(size_t) is called" << std::endl;
#endif
}

Line::~Line() {
#ifdef debug
    std::cout << "Destructor is called" << std::endl;
#endif
};

size_t Line::size() const {
#ifdef debug
    std::cout << "size() is called" << std::endl;
#endif
    return points.size();
}

const float & Line::x(size_t it) const {
#ifdef debug
    std::cout << "const x is called" << std::endl;
#endif
    return points[it].first;
}

float & Line::x(size_t it) {
#ifdef debug
    std::cout << "x is called" << std::endl;
#endif
    return points[it].first;
}

const float & Line::y(size_t it) const {
#ifdef debug
    std::cout << "const y is called" << std::endl;
#endif
    return points[it].second;
}

float & Line::y(size_t it) {
#ifdef debug
    std::cout << "y is called" << std::endl;
#endif
    return points[it].second;
}
