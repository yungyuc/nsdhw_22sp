#include <iostream>
#include <math.h>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

/*
Definition: cosθ = a.b / |a||b|
LET a=(x1,y1), b=(x2,y2)

a.b=(x1x2+y1y2)
|a|=√(x1^2+y1^2), |b|=√(x2^2+y2^2)
=> cos<a,b>=(x1x2+y1y2)/[√(x1^2+y1^2)*√(x2^2+y2^2)]
*/

float cal_angle(vector<float> v1, vector<float> v2)
{
    float len_v1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2));
    float len_v2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2));
    if (len_v1 == 0.0 || len_v2 == 0.0)
        throw std::length_error("vector length is zero.");

    float dot = v1[0] * v2[0] + v1[1] * v2[1];
    float cos_vec = dot / (len_v1 * len_v2);

    if (cos_vec >= 1.0)
        cos_vec = 1;
    else if (cos_vec <= -1.0)
        cos_vec = -1;

    return acos(cos_vec);
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "cal_angle";
    m.def("cal_angle",
          &cal_angle,
          "Calculate the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate.");
}
