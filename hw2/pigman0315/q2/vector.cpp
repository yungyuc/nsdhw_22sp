#include <iostream>
#include <pybind11/pybind11.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

#define PI 3.14159265359
class Line
{
public:
    Line() { m_size = 0; }
    Line(size_t size);
    ~Line() {m_size = 0; m_nodes.clear();}
    size_t size() const { return m_size; }
    void x(size_t it,float val) { check_range(it); m_nodes[it].first = val; }
    void y(size_t it,float val) { check_range(it); m_nodes[it].second = val; }
    std::pair<float,float> get_vec();

private:
    // private helper
    void check_range(size_t it) const {
        if(it >= m_size) {throw std::out_of_range("Line index out of range");}
    }

    // Member data.
    size_t m_size;
    std::vector<std::pair<float,float>> m_nodes;

}; /* end class Line */
std::pair<float,float> Line::get_vec(){
    if(m_size < 2){
        throw std::out_of_range("Not enough nodes");
    }
    std::pair<float,float> vec;
    vec.first = (m_nodes[0].first - m_nodes[1].first);
    vec.second = (m_nodes[0].second - m_nodes[1].second);
    if(vec.first == 0.0 && vec.second == 0.0){
        throw std::length_error("Zero length of vector");
    }
    return vec;
}
Line::Line(size_t size){
    m_size = size;
    m_nodes.clear();
    for(size_t i = 0;i < size;i++){
        std::pair<float,float> tmp_p;
        m_nodes.push_back(tmp_p);
    }
}
double calc_radius(Line l1, Line l2)
{
    std::pair<float,float> vec1 = l1.get_vec();
    std::pair<float,float> vec2 = l2.get_vec();
    double ab,a1,b1,cos,radius;
    ab=vec1.first*vec2.first+vec1.second*vec2.second;
    a1=std::sqrt(vec1.first*vec1.first+vec1.second*vec1.second);
    b1=std::sqrt(vec2.first*vec2.first+vec2.second*vec2.second);
    cos=ab/(a1*b1);
    cos = std::min(1.0,std::max(cos,-1.0));
    radius = std::acos(cos);
    return radius;
}

// int main(){
//     Line l1(2);
//     Line l2(2);
//     l1.x(0,0);l1.y(0,0);l1.x(1,1);l1.y(1,1);
//     l2.x(0,1);l2.y(0,1);l2.x(1,2);l2.y(1,2);
//     double r = calc_radius(l1,l2);
//     std::cout << r << std::endl;
// }

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11: calculate two lines' radius";      // module doc string
  m.def("calc_radius",                              // function name
        &calc_radius,                               // function pointer
        "A function calculate two lines' radius" //function doc string
       );
  pybind11::class_<Line>(m, "Line")
        .def(pybind11::init<size_t>())
        .def( "size", &Line::size)
        .def( "get_vec", &Line::get_vec)
        .def( "x", &Line::x)
        .def( "y", &Line::y);
}