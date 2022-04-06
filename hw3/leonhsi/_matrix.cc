#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include "_matrix.h"

using namespace std;
namespace py = pybind11;

bool operator== (Matrix const &mat1, Matrix const &mat2){
	if(mat1.ncol() != mat2.ncol() || mat1.nrow() != mat2.ncol()) return false;

	for(size_t i=0; i<mat1.nrow(); i++){
		for(size_t j=0; j<mat1.ncol(); j++){
			if(mat1(i, j) != mat2(i, j)) return false;
		}
	}

	return true;
}

bool operator!= (Matrix const &mat1, Matrix const &mat2){
	return !(mat1 == mat2);
}

void can_multiply(Matrix const &mat1, Matrix const &mat2){
	if(mat1.ncol() != mat2.nrow()){
		throw out_of_range("the number of matrix 1's column does not equal to matrix 2's row");
	}
}

Matrix multiply_naive(Matrix const &mat1, Matrix &mat2){
	can_multiply(mat1, mat2);

	Matrix ret(mat1.nrow(), mat2.ncol());

	const size_t nrow1 = mat1.nrow();
	const size_t ncol1 = mat1.ncol();
	const size_t ncol2 = mat2.ncol();

	for (size_t i=0; i<nrow1; ++i)
	{
		const size_t base1 = i * ncol1;
		for (size_t k=0; k<ncol2; ++k)
		{
			double v = 0;
			for (size_t j=0; j<ncol1; ++j)
			{
				v += mat1.m_buffer[base1 + j] * mat2.m_buffer[j*ncol2 + k];
			}
			ret.m_buffer[base1 + k] = v;
		}
	}

	return ret;
}

Matrix multiply_tile(const Matrix &a, const Matrix &b, size_t blocksize)
{
    can_multiply(a, b);

    Matrix c(a.nrow(), b.ncol());

    for(size_t blocki=0 ; blocki<a.nrow() ; blocki+=blocksize)
    {
        size_t i_bound = std::min( blocki+blocksize, a.nrow() );
        for(size_t blockj=0 ; blockj<b.ncol() ; blockj+=blocksize)
        {
            size_t j_bound = std::min( blockj+blocksize, b.ncol() );
            for(size_t blockk=0 ; blockk<a.ncol() ; blockk+=blocksize)
            {
                size_t k_bound = std::min( blockk+blocksize, a.ncol() );
                for(size_t k=blockk ; k<k_bound ; k++)
                {
                    for(size_t i=blocki ; i<i_bound ; i++)
                    {
                        for(size_t j=blockj ; j<j_bound ; j++)
                        {
                            c(i,j) += a(i,k)*b(k,j);
                        }
                    }
                }
            }
        }
    }

    return c;
}

Matrix multiply_mkl(Matrix &a, Matrix &b){
	can_multiply(a, b);

	Matrix ret(a.nrow(), b.ncol());

	const size_t M = a.nrow();
	const size_t N = b.ncol();
	const size_t K = a.ncol();
	const double alpha = 1.0, beta = 0.0;

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M, N, K, alpha, a.val_addr(), M, b.val_addr(), N, beta, ret.val_addr(), N);

	return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix-multiplication function test and performance.";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<size_t, size_t>())
        .def(py::self == py::self)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", &Matrix::getvalue)
        .def("__setitem__", &Matrix::setvalue);
}
