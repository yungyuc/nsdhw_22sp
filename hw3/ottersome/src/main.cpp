#include "main.h"
#include "matrix.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;


PYBIND11_MODULE(example, m) {
    //py::class_<Animal>(m, "Animal")
    //.def("go", &Animal::go);

    //py::class_<Dog, Animal>(m, "Dog")
    //.def(py::init<>());

    //m.def("call_go", &call_go);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def_buffer([](Matrix &m) -> py::buffer_info {
                return py::buffer_info(
                        m.data(),                               /* Pointer to buffer */
                        sizeof(float),                          /* Size of one scalar */
                        py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
                        2,                                      /* Number of dimensions */
                        { m.nrow(), m.ncol() },                 /* Buffer dimensions */
                        { sizeof(float) * m.ncol(),             /* Strides (in bytes) for each index */
                        sizeof(float) }
                        );
                });
}




//int main(int argc, char ** argv)
//{
    //std::cout << ">>> A(2x3) times B(3x2):" << std::endl;
    //Matrix mat1(2, 3, std::vector<double>{1, 2, 3, 4, 5, 6});
    //Matrix mat2(3, 2, std::vector<double>{1, 2, 3, 4, 5, 6});

    //Matrix mat3 = mat1 * mat2;

    //std::cout << "matrix A (2x3):" << mat1 << std::endl;
    //std::cout << "matrix B (3x2):" << mat2 << std::endl;
    //std::cout << "result matrix C (2x2) = AB:" << mat3 << std::endl;

    //std::cout << ">>> B(3x2) times A(2x3):" << std::endl;
    //Matrix mat4 = mat2 * mat1;
    //std::cout << "matrix B (3x2):" << mat2 << std::endl;
    //std::cout << "matrix A (2x3):" << mat1 << std::endl;
    //std::cout << "result matrix D (3x3) = BA:" << mat4 << std::endl;

    //return 0;
//}
