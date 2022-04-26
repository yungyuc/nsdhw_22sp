#include <mkl.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <pybind11/pybind11.h>

#include "allocator.hpp"

namespace pybind = pybind11;

class Matrix
{
    public:
        Matrix(size_t const& row, size_t const& col);
        Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec);
        Matrix(const Matrix &matrix);
        Matrix& operator= (const Matrix& matrix);
        Matrix(Matrix && mat);
        ~Matrix()
        {
            vec_.clear();
            vec_.shrink_to_fit();
        };
        
        Matrix& operator= (std::vector<double> const& vec);
        double  operator() (size_t const& row, size_t const& col) const;
        double & operator() (size_t const& row, size_t const& col);
        bool operator== (Matrix const &matrix) const ;
        

        friend Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2);
        friend Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize);
        friend Matrix multiply_mkl(Matrix & matrix1, Matrix & matrix2);
        
        size_t nrow() const { return number_row; }
        size_t ncol() const { return number_col; }
        double * get_matrix_buffer();
        double get_element(std::pair<size_t, size_t> index);
        void set_element(std::pair<size_t, size_t> index, const double& item);

    private:
        size_t number_row;
        size_t number_col;
        std::vector<double, MyAllocator<double>> vec_;
};

void validate_multiplication(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2);
Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize);
Matrix multiply_mkl(Matrix & matrix1, Matrix & matrix2);
void initialize(Matrix & mat);
std::size_t bytes();
std::size_t allocated();
std::size_t deallocated();

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("initialize", &initialize);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);

    pybind::class_<Matrix>(m, "Matrix")
        .def(pybind::init<int, int>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def("__setitem__", &Matrix::set_element)
        .def("__getitem__", &Matrix::get_element);   
}