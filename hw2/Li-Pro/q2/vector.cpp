#include <cmath>
#include <exception>

#include <pybind11/pybind11.h>

constexpr double eps = 1e-6;

double
vec_cross(double ax, double ay, double bx, double by)
{
    return ax*by - ay*bx;
}

double
vec_length(double x, double y)
{
    return sqrt(x*x + y*y);
}

double
get_rad(double ax, double ay, double bx, double by)
{
    if (vec_length(ax, ay) < eps)
    {
        throw std::invalid_argument("1st vector has zero-length");
    }

    if (vec_length(bx, by) < eps)
    {
        throw std::invalid_argument("2nd vector has zero-length");
    }

    // what do you mean by precision :P
    double cosrad = vec_cross(ax, ay, bx, by) / vec_length(ax, ay) / vec_length(bx, by);
    return abs(asin(cosrad));
}

PYBIND11_MODULE(libvector, m) {
    m.def("_get_rad", &get_rad);
}
