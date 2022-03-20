#include <pybind11/pybind11.h>
#include <stdexcept>
#include <utility>
#include <limits>
namespace py = pybind11;



double calc_angle(const std::pair<double, double> a, const std::pair<double, double> b) {
  double a_len = hypot(a.first, a.second);
  double b_len = hypot(b.first, b.second);
  double eps = std::numeric_limits<double>::epsilon();
  if (a_len < eps || b_len < eps) {
    throw std::invalid_argument("invalid input");
  }
  double c_len = hypot(a.first - b.first, a.second - b.second);
  return acos((a_len * a_len + b_len * b_len - c_len * c_len) / 2 / a_len / b_len);
}



PYBIND11_MODULE(_vector, m) {
    m.def("calc_angle", &calc_angle, "Calculates the angle between two vectors in the 2-dimensional Cartesian coordinate system");
}
