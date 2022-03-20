#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>

namespace py = pybind11;

class Vector2D
{
private:
    double _x;
    double _y;
public:
    Vector2D(double x = 0,double y = 0):_x(x),_y(y){}
    double getLength() const
    {
        return sqrt(_x*_x + _y*_y);
    }
    double dotProduct(const Vector2D& rhs) const
    {
        return _x*rhs._x + _y*rhs._y;
    }
};

double calcAngle(const Vector2D& v1,const Vector2D& v2)
{
    double d = v1.getLength() * v2.getLength();
    if(d == 0.0)
        throw std::overflow_error("Zero length vector arguments");
    return acos(v1.dotProduct(v2) / d);
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system";
    py::class_<Vector2D>(m, "Vector2D")
        .def(py::init<double,double>(),
            py::arg("x") = 0.0,
            py::arg("y") = 0.0)
        .def("getLength",&Vector2D::getLength)
        .def("dotProduct",&Vector2D::dotProduct,
            py::arg("rhs"));
    m.def("calcAngle",&calcAngle,
        py::arg("v1"),
        py::arg("v2"));
}