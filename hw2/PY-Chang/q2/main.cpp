#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct VectorCord{
	float x;
	float y;

	VectorCord (float x, float y){
		this->x = x;
		this->y = y;
	}
};

float vector_angle(VectorCord a, VectorCord b){
	
	float dotProduct = (a.x*b.x) + (a.y*b.y);
	
	float aLength = sqrt( ((a.x*a.x) + (a.y*a.y)) );
	float bLength = sqrt( ((b.x*b.x) + (b.y*b.y)) );
	
	if (aLength==0 || bLength == 0){
		//std::cout<<"Invalid input (zero length)."<<std::endl;
		throw std::invalid_argument("Invalid input (zero length).");
		return -1;
	}
	
	float cosAngle = dotProduct / (aLength * bLength);
	//std::cout<<cosAngle<<std::endl;
	
	if (cosAngle > 1.0) cosAngle = 1.0;
	else if (cosAngle < -1.0)
		cosAngle = -1.0;
	
	//cout<<acosf(cosAngle)<<endl;
	
	return acosf(cosAngle);
}

PYBIND11_MODULE(_vector, m){
    m.doc() = "pybind11 example";
    py::class_<VectorCord>(m, "VectorCord")
        .def(py::init<float, float>());

    py::register_exception<std::invalid_argument>(m, "invalid_argument");

    m.def("vector_angle", &vector_angle);
}