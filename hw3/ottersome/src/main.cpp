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

Matrix multiply_naive(Matrix &m1, const Matrix &m2){
    Matrix retMat = m1.multiply_naive(m2);
    std::cout << "Before returning retMat we check mrow:"<<retMat.get_nrow()<<std::endl;
    return retMat;
}
Matrix multiply_mkl(Matrix &m1, const Matrix &m2){
    return m1.multiply_mkl(m2);
}

PYBIND11_MODULE(_matrix, m) {

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def("print_vals",&Matrix::print_vals)
        //.def("nrow",&Matrix::nrow)
        //.def("ncol",&Matrix::ncol)
        .def_property_readonly("nrow",&Matrix::get_nrow)
        .def_property_readonly("ncol",&Matrix::get_ncol)
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
        //.def("multiply_naive",&multiply_naive)
        .def("multiply_tile",&Matrix::multiply_tile)
        //.def("multiply_mkl",&multiply_mkl)
    //TODO i need to verify the correctness of this
        .def_buffer([](Matrix &m) -> py::buffer_info {
                return py::buffer_info(
                        m.data(),                               /* Pointer to buffer */
                        sizeof(float),                          /* Size of one scalar */
                        py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
                        2,                                      /* Number of dimensions */
                        { m.get_nrow(), m.get_ncol() },                 /* Buffer dimensions */
                        { sizeof(float) * m.get_ncol(),             /* Strides (in bytes) for each index */
                        sizeof(float) }
                        );
                });
    m.def("ident",&ident,"Function initializes identity matrix");
    m.def("multiply_naive",&multiply_naive,"Naive multiplication");
    m.def("multiply_mkl",&multiply_mkl,"Multiply using MKL method");
}
