#include <utility>
#include <math.h>
#include <pybind11/pybind11.h>
// #include <iostream>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

float angleofTwoVector(std::pair<float, float> v1, std::pair<float, float> v2) {
    return acos((v1.first * v2.first + v1.second * v2.second) / sqrt((v1.first*v1.first+v1.second*v1.second)*(v2.first*v2.first+v2.second*v2.second)));
}

PYBIND11_MODULE(angle, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    // m.def("add", &add, "A function that adds two numbers");
    m.def("angleofTwoVector", &angleofTwoVector, "A function that adds two numbers");
}

// int main() {
//     std::cout << angleofTwoVector({1,0}, {0, 1}) << std::endl;
// }