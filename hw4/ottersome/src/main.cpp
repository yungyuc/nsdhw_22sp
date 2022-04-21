#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <iostream>
#include <tuple>

#include "allocator.h"
#include "main.h"
#include "matrix.h"


namespace py = pybind11;

//Mostly for tests
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

Matrix multiply_naive(const Matrix &m1, const Matrix &m2){
    return m1.multiply_naive(m2);
}
Matrix multiply_tile(const Matrix &m1, const Matrix &m2,size_t tile_size){
    return m1.multiply_tile(m2, tile_size);
}
Matrix multiply_mkl(Matrix &m1, const Matrix &m2){
    return m1.multiply_mkl(m2);
}

PYBIND11_MODULE(_matrix, m) {

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        //.def(py::init<size_t, size_t>(),[](size_t m, size_t n){
                //return Matrix(m,n,customAllocator);
                //})
        .def("print_vals",&Matrix::print_vals)
        .def(py::self == py::self)
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

            //TODO Implement this for when we want to 
            //deal with slices
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
        .def("multiply_tile",&Matrix::multiply_tile);
    m.def("ident",&ident,"Function initializes identity matrix");
    m.def("multiply_naive",&multiply_naive,"Naive multiplication");
    m.def("multiply_mkl",&multiply_mkl,"Multiply using MKL method");
    m.def("multiply_tile",&multiply_tile,"Multiply using Tile method");
    m.def("bytes",[](){return ByteCounter::cur_alloc_bytes();},"Total bytes currently allocated");
    m.def("allocated",[](){return ByteCounter::tot_alloc_bytes();},"Total bytes ever allocated");
    m.def("deallocated",[](){return ByteCounter::tot_dealloc_bytes();},"Total bytes ever deallocated");
}
