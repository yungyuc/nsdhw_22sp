#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Line
{
public:
    // Basic constructor
    Line();
    Line(Line const &);
    Line(Line &&);
    Line &operator=(Line const &);
    Line &operator=(Line &&);

    // Custom constructor
    Line(size_t size);

    // Destructor
    ~Line();

    // Accessor
    size_t size() const;
    const float& x(size_t it) const;
    float& x(size_t it);
    const float& y(size_t it) const;
    float& y(size_t it);

private:
    vector<float> coordinate;
};