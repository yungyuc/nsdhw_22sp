//
//  matrix.hpp
//  nsd-hw3
//
//  Created by MinChunCho on 2022/3/25.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <mkl.h>

namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t const& row, size_t const& col);
    Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec);
    Matrix(Matrix const& mat);
    Matrix& operator=(Matrix const& mat);
    Matrix(Matrix && mat);
    Matrix& operator=(Matrix && mat);
    ~Matrix(){
        vec_.clear();
        vec_.shrink_to_fit();
    }
    
    Matrix& operator=(std::vector<double> const& vec);
    double& operator()(size_t const& i, size_t const& j);
    double operator()(size_t const& i, size_t const& j) const;
    bool operator==(Matrix const& mat) const;
    friend std::ostream& operator<<(std::ostream& os, Matrix mat);
    friend Matrix multiply_naive(Matrix const& ma, Matrix const& mb);
    friend Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize);
    friend Matrix multiply_mkl(Matrix& ma, Matrix& mb);

    size_t row() const { return row_; }
    size_t col() const { return col_; }
    size_t size() const { return row_*col_; }
    double* addr() { return &vec_[0]; }
    double get_element(std::pair<size_t, size_t> index);
    void set_element(std::pair<size_t, size_t> index, const double& item);
    std::string get_matrix_str();

private:
    bool bound_check(size_t const& i, size_t const& j);

    size_t row_;
    size_t col_;
    std::vector<double> vec_;
};

Matrix multiply_naive(Matrix const& ma, Matrix const& mb);
Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize);
Matrix multiply_mkl(Matrix& ma, Matrix& mb);

PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t const&, size_t const&>())
        .def(py::init<size_t const&, size_t const&, std::vector<double> const&>())
        .def(py::self == py::self) // operator overloading
        .def_property_readonly("nrow", &Matrix::row) // the following functions are made for validate.sh
        .def_property_readonly("ncol", &Matrix::col)
        .def("__setitem__", &Matrix::set_element)
        .def("__getitem__", &Matrix::get_element)
        .def("__repr__", &Matrix::get_matrix_str);
        
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
};

#endif /* matrix_hpp */
