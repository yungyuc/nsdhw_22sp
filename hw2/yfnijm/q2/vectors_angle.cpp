#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

float vectors_angle(std::vector<float> v1, std::vector<float> v2){
    try{
        size_t zero1 = std::count(v1.begin(), v1.end(), 0.0);
        size_t zero2 = std::count(v2.begin(), v2.end(), 0.0);

        if(v1.size() != v2.size() || zero1 == v1.size() || zero2 == v2.size()){
            throw std::invalid_argument("Invalid input");
        }
    }
    catch(const char* msg){

    }


    float len1 = 0.0;
    for(auto& cor : v1)
        len1 += cor * cor;
    len1 = std::sqrt(len1);

    float len2 = 0.0;
    for(auto& cor : v2)
        len2 += cor * cor;
    len2 = std::sqrt(len2);

    float dot = 0.0;
    
    for(size_t i = 0; i < v1.size(); i++)
        dot += v1[i] * v2[i];

    return acos(dot / len1 / len2);
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 plugin";
  m.def("vectors_angle",                             
        &vectors_angle,                             
        "Caulculate the angle of two vectors, which input is: v1, v2"
       );
}
