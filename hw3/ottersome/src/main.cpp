#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <iostream>
#include <tuple>

#include "main.h"
#include "matrix.h"


namespace py = pybind11;

Matrix ident(size_t row,size_t col){

    if (row != col)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix mat(row,col);
    mat.zero_out();

    for(size_t i=0;i<row;i++){
        mat(i,i) = 1;
    }

    return mat;
}

PYBIND11_MODULE(_matrix, m) {

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def("print_vals",&Matrix::print_vals)
        .def("nrow",&Matrix::nrow)
        .def("ncol",&Matrix::ncol)
        .def("__getitem__", [](const Matrix &m, const py::slice & slice) {
            py::ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (!slice.compute(m.size(), &start, &stop, &step, &slicelength)) {
                throw py::error_already_set();
            }
            int istart = static_cast<int>(start);
            int istop = static_cast<int>(stop);
            int istep = static_cast<int>(step);

            //TODO change this for when we need it 
            return std::make_tuple(istart, istop, istep);
        })
        .def("__getitem__", [](const Matrix &m, std::tuple<size_t,size_t> indices) {
                size_t row, col;
                std::tie(row, col) = indices;
            return m(row,col);
        })
        .def("__setitem__", [](Matrix &m, std::tuple<size_t,size_t> indices, const double value) {
                size_t row, col;
                std::tie(row, col) = indices;
                m(row,col) = value;
        })
        .def("multiply_naive",&Matrix::multiply_naive)
        .def("multiply_tile",[](Matrix &m1, const Matrix & m2, size_t tile_size){
                return m1.multiply_tile(m2,tile_size);
                })
    //TODO i need to verify the correctness of this
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
    m.def("ident",&ident,"Function initializes identity matrix");
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
