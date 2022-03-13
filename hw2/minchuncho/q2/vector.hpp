#ifndef vec_hpp
#define vec_hpp

#define PI 3.14159265

#include <cmath>
#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vec {
public:
    // constructors
    Vec();
    Vec(double const&, double const&);
    // copy constructor
    Vec(Vec const &);
    // copy assignment
    // Vec & operator=(Vec const&);
    // move constructor
    // Vec(Vec &&);
    // move assignment
    // Vec & operator=(Vec &&);
    //destructor
    ~Vec();
    // member functions
    static double angle(Vec const&, Vec const&);
    void show();
private:
    double x_;
    double y_;
};

inline
Vec::Vec(){}

inline
Vec::Vec(double const& x, double const& y): x_(x), y_(y){}

inline
Vec::~Vec(){}

PYBIND11_MODULE(_vector, m) {
    py::class_<Vec>(m, "Vec")
        .def(py::init())
        .def(py::init<double const&, double const&>())
        .def(py::init<Vec const&>())
        .def("show", &Vec::show)
        .def_static("angle", static_cast<double (*)(Vec const&, Vec const&)>(&Vec::angle));
};

#endif /* line_hpp */
