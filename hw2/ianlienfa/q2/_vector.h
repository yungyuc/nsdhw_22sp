#ifndef _VECTOR_H
#define _VECTOR_H
#include <iostream>
#include <pybind11/pybind11.h>
#include <math.h>


class Vector {
public:    
    float x;
    float y;
    Vector(float x, float y): x(x), y(y) {}
    float size() const;
    static float getAngle(Vector const &, Vector const &);
};

PYBIND11_MODULE(_vector, m) {
    m.attr("info") = "vector class";    
    pybind11::class_<Vector>(m, "Vector")
        .def(pybind11::init<float, float>())
        .def("size", &Vector::size)
        .def_static("getAngle", &Vector::getAngle);
}

#endif