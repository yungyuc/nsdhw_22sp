#include "StopWatch.hpp"

#include <mkl.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <pybind11/pybind11.h>

namespace pybind = pybind11;

class Matrix 
{
    public:

        Matrix(size_t init_row, size_t init_col): number_row(init_row), number_col(init_col)
        {
            matrix_buffer = new double[init_row*init_col]();
        }

        ~Matrix()
        {
            delete [] matrix_buffer;
        }

        Matrix(const Matrix &matrix)
        {
            number_row = matrix.number_row;
            number_col = matrix.number_col;
            matrix_buffer = new double[number_row * number_col](); 
            memcpy(matrix_buffer, matrix.matrix_buffer, number_row * number_col*sizeof(double));
        }

        bool operator== (Matrix const &matrix) const 
        {
            for (size_t i = 0; i < number_row; i++) 
                for (size_t j = 0; j < number_col; j++)
                    if (matrix_buffer[i*number_col + j] != matrix(i, j)) return false;

            return true;
        }

        Matrix& operator= (const Matrix& matrix) 
        {
            matrix_buffer = matrix.matrix_buffer;
            return *this;
        }

        double  operator() (size_t row, size_t col) const 
        { 
            return matrix_buffer[row*number_col + col]; 
        }
        double & operator() (size_t row, size_t col)
        { 
            return matrix_buffer[row*number_col + col]; 
        }
        double * get_matrix_buffer() const 
        { 
            return matrix_buffer; 
        }
        size_t nrow() const { return number_row; }
        size_t ncol() const { return number_col; }

    private:

        size_t number_row;
        size_t number_col;
        double *matrix_buffer;

};

Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2)
{

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    for (size_t row = 0; row < ret.nrow(); row++) 
    {
        for (size_t column = 0; column < ret.ncol(); column++) 
        {
            double value = 0;
            for (size_t k = 0; k < matrix1.ncol(); k++) {
                value += matrix1(row, k) * matrix2(k, column);
            }
            ret(row, column) = value;
        }
    }

    return ret;

}

Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize)
{

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    for (size_t row = 0; row < matrix1.nrow(); row += tilesize) 
    {
        for (size_t col = 0; col < matrix2.ncol(); col += tilesize) 
        {
            for (size_t inner = 0; inner < matrix1.ncol(); inner += tilesize) 
            {
                for (size_t k = inner; k < std::min(matrix1.ncol(), inner + tilesize); k++) 
                {
                    for (size_t i = row; i < std::min(matrix1.nrow(), row + tilesize); i++) 
                    {
                        for (size_t j = col; j < std::min(matrix2.ncol(), col + tilesize); j++)
                        {
                            ret(i, j) += matrix1(i, k) * matrix2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;

}

Matrix multiply_mkl(Matrix const & matrix1, Matrix const & matrix2)
{

    Matrix ret = Matrix(matrix1.nrow(), matrix2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        matrix1.nrow(),
        matrix2.ncol(),
        matrix1.ncol(),
        1.0,
        matrix1.get_matrix_buffer(),
        matrix1.ncol(),
        matrix2.get_matrix_buffer(),
        matrix2.ncol(),
        0.0,
        ret.get_matrix_buffer(),
        ret.ncol()
    );

    return ret;
} 

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiply";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    pybind::class_<Matrix>(m, "Matrix")
        .def(pybind::init<int, int>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def("assign", &Matrix::operator=)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val) { return mat(idx.first, idx.second) = val; })
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx) { return mat(idx.first, idx.second); });
}