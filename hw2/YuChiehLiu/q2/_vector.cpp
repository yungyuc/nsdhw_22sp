#include <iostream>
#include <cmath>
#include <vector>
#include <math.h>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

//using namespace std;

float Angle_Calculator(std::vector<float> vec1, std::vector<float> vec2)
{
    float dot = vec1.at(0)*vec2.at(0) + vec1.at(1)*vec2.at(1);
    float len_vec1 = std::sqrt(vec1.at(0)*vec1.at(0) + vec1.at(1)*vec1.at(1));
    float len_vec2 = std::sqrt(vec2.at(0)*vec2.at(0) + vec2.at(1)*vec2.at(1));
    
    if( len_vec1==0.0 || len_vec2==0.0)
        throw std::length_error("Length cannot be zero.");
    
    float cos = dot/len_vec1/len_vec2;
    
    if( cos >= 1.0 ) cos = 1.0;
    else if (cos <= -1.0) cos = -1.0;
       
    return std::acos(cos);
}

PYBIND11_MODULE(_vector, m)
{
    m.def("get_angle", &Angle_Calculator, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.");
}