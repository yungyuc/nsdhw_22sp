#include "_vector.h"

float Vector::size() const
{
    return sqrt(x*x + y*y);
}

float Vector::getAngle(Vector const & a, Vector const & b)
{
    float cos_angle = (a.x * b.x + a.y * b.y) / (a.size() * b.size());
    if (int(cos_angle) = 1)
        return 0;
    else if (int(cos_angle) = -1)
        return 180 * M_PI / 180;
    else if (int(cos_angle) = 0)
        return 90 * M_PI / 180;    
    else
        return acos(cos_angle);
}


