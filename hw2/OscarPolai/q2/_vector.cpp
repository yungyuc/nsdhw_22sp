#include <iostream>
#include <stdexcept>
#include <math.h>
#include <pybind11/pybind11.h>
#include <vector>
#include <pybind11/stl.h>


float get_angle(std::vector<float> v1, std::vector<float> v2){
    if ((v1[0] == 0.0 && v1[1] == 0.0) || (v2[0] == 0.0 && v2[1] == 0.0))
         throw std::length_error("invalid input , zero length");

    float len1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2));
    float len2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2));

    float dot = v1[0] * v2[0] + v1[1] * v2[1];
    float cos_vec = dot / (len1 * len2);

    return acos(cos_vec);
 }


 PYBIND11_MODULE(_vector, m){
     m.doc() = "get_angle";
     m.def("get_angle", &get_angle,
           "Calculate the angle between two vectors.");
 }