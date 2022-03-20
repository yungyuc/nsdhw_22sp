#ifndef LINE_H
#define LINE_H
#include <vector>
#include <stdexcept>
using namespace std;

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
    vector<float> x_;
    vector<float> y_;
    size_t size_;
    // Member data. /
}; /* end class Line */

#endif