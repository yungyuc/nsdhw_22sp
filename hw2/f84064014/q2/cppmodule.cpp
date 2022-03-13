#include <iostream>
#include <math.h>
#include <pybind11/pybind11.h>

float get_angle(float x1, float y1, float x2, float y2)
{

	if ( (x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0) )
		throw std::runtime_error("get zero length vector");

	float abs1 = sqrt( pow(x1, 2) + pow(y1, 2) );
	float abs2 = sqrt( pow(x2, 2) + pow(y2, 2) );
	float dot_product = x1 * x2 + y1 * y2;

	return acos(dot_product/(abs1 * abs2));
}

PYBIND11_MODULE(_vector, m){
	m.doc() = "pybind11 example";
	m.def("py_get_angle", &get_angle, "get angle in radian",
		pybind11::arg("x1") = 0, pybind11::arg("y1") = 0,
		pybind11::arg("x2") = 0, pybind11::arg("y2") = 0);
}
