#include <utility>
#include <math.h>
#include <pybind11/pybind11.h>
// #include <iostream>
// #include <iomanip>

namespace py = pybind11;

double angleofTwoVector(std::pair<double, double> v1, std::pair<double, double> v2) {
    return acos((v1.first * v2.first + v1.second * v2.second) / sqrt((v1.first*v1.first+v1.second*v1.second)*(v2.first*v2.first+v2.second*v2.second)));
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 wrap vector angle"; // optional module docstring

    m.def("angleofTwoVector", &angleofTwoVector, "A function that calculate degree between two vectors");
}

// int main() {
//     std::cout << std::setprecision(15) << angleofTwoVector({1,0}, {0, 1}) << std::endl;
// }