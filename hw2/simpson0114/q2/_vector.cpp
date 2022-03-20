#include <cstdint>
#include <stdexcept>
#include <math.h>
#include <iostream>
#include <pybind11/pybind11.h>

class Vector
{
public:
    Vector() = default;

    Vector(float x, float y): m_x(x), m_y(y){}

    void setx(float x) {
        m_x = x;
    }

    const float getx() {
        return m_x;
    }

    void sety(float y) {
        m_y = y;
    }

    const float gety() {
        return m_y;
    }
private:
    float m_x;
    float m_y;
};

double cal_angle(Vector vec1, Vector vec2) {
    if ((vec1.getx() == 0 && vec1.gety() == 0) || (vec2.getx() == 0 && vec2.gety() == 0)) {
        throw std::invalid_argument("Invalid input (zero length).");
        return -1;
    }
    double dot = vec1.getx() * vec2.getx() + vec1.gety() * vec2.gety();
    double det = vec1.getx() * vec2.gety() + vec1.gety() * vec2.getx();
    double angle = atan2(det, dot);
    return angle;
}

PYBIND11_MODULE(_vector, m) {
    m.def("python_cal_angle", &cal_angle, "calculate angle between two vectors");
    pybind11::class_<Vector>(m, "Vector")
        .def(pybind11::init<float , float>())
        .def("setx", &Vector::setx)
        .def("getx", &Vector::getx)
        .def("sety", &Vector::sety)
        .def("gety", &Vector::gety);
    pybind11::register_exception<std::invalid_argument>(m, "invalid_argument");
}

// int main()
// {
//     Vector vec1(sqrt(3), 1);
//     Vector vec2(sqrt(3), 0);
//     std::cout << cal(vec1, vec2) << std::endl;
// }

// g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` cal.cpp -o cal`python3-config --extension-suffix` -I /usr/include/python3.8/