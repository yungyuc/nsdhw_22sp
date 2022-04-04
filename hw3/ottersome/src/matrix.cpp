#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <math.h>
#ifdef __unix__
#include "mkl.h"
#else
#include "f77blas.h"
#endif

namespace py = pybind11;

#include "matrix.h"


Matrix Matrix::multiply_mkl(const Matrix & mat2){
    if (this->ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix ret(this->nrow, mat2.ncol);

    //TODO, not so sure about teh values of these alphas
    cblas_dgemm(
            CblasRowMajor,
            CblasNoTrans,
            CblasNoTrans,
            this->nrow,
            mat2.ncol,
            this->ncol,
            1.0,
            this->m_buffer,
            this->m_ncol,
            mat2.data(),
            mat2.ncol,
            1.0,
            ret.data(),
            ret.ncol
            );

    return ret;

}
Matrix Matrix::multiply_naive(const Matrix & mat2)
{
    if (this->ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(this->nrow, mat2.ncol);

    for (size_t i=0; i<ret.nrow; ++i)
    {
        for (size_t k=0; k<ret.ncol; ++k)
        {
            double v = 0;
            for (size_t j=0; j<this->ncol; ++j)
            {
                v += (*this)(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    std::cout <<" Seems liek multiplication went fine"<<std::endl
;
    return ret;
}

Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow),m_ncol(ncol),nrow(m_nrow),ncol(m_ncol)
{
    reset_buffer(nrow, ncol);
}

Matrix::Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
    : m_nrow(nrow),m_ncol(ncol),nrow(m_nrow),ncol(m_ncol)

{
    reset_buffer(nrow, ncol);
    (*this) = vec;
}

Matrix & Matrix::operator=(std::vector<double> const & vec)
{
    if (size() != vec.size())
    {
        throw std::out_of_range("number of elements mismatch");
    }

    size_t k = 0;
    for (size_t i=0; i<nrow; ++i)
    {
        for (size_t j=0; j<ncol; ++j)
        {
            (*this)(i,j) = vec[k];
            ++k;
        }
    }

    return *this;
}

Matrix::Matrix(Matrix const & other)
    : nrow(other.nrow),ncol(other.ncol)

{
    reset_buffer(other.nrow, other.ncol);
    for (size_t i=0; i<nrow; ++i)
    {
        for (size_t j=0; j<ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
}

Matrix & Matrix::operator=(Matrix const & other)
{
    if (this == &other) { return *this; }
    if (nrow != other.nrow || ncol != other.ncol)
    {
        reset_buffer(other.nrow, other.ncol);
    }
    for (size_t i=0; i<nrow; ++i)
    {
        for (size_t j=0; j<ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}

Matrix::Matrix(Matrix && other)
    : nrow(other.nrow),ncol(other.ncol)

{
    reset_buffer(0, 0);
    //nrow = other.nrow;
    //ncol = other.ncol;
    //std::swap(nrow, other.nrow);
    //std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
}

Matrix & Matrix::operator=(Matrix && other)
{
    if (this == &other) { return *this; }
    reset_buffer(0, 0);
    //m_nrow = other.nrow;
    //m_ncol = other.ncol;
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_nrow, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

Matrix::~Matrix()
{
    reset_buffer(0, 0);
}

double Matrix::operator() (size_t row, size_t col) const
{
    return m_buffer[index(row, col)];
}
double & Matrix::operator() (size_t row, size_t col)
{
    return m_buffer[index(row, col)];
}

//size_t Matrix::nrow() const { return m_nrow; }
//size_t Matrix::ncol() const { return m_ncol; }
size_t Matrix::size() const { return m_ncol*m_nrow; }

double Matrix::buffer(size_t i) const { return m_buffer[i]; }
std::vector<double> Matrix::buffer_vector() const
{
    return std::vector<double>(m_buffer, m_buffer+size());
}


double * Matrix::data(){
    return m_buffer;
}
double * Matrix::data() const {
    return m_buffer;
}
void Matrix::zero_out(){
    memset(m_buffer,0,m_nrow*m_ncol*sizeof(double));
}


void Matrix::print_vals(){

    if(m_nrow > 100 or m_ncol){
        for(size_t r=0;r< m_nrow;r++){
            for(size_t c=0;c< m_ncol;c++){
                py::print((*this)(r,c));
            }
            py::print("\n");
        }
    }else{
        py::print("Matrix too big to print");
    }
}


/*********************
    Private Members
*********************/

size_t Matrix::index(size_t row, size_t col) const
{
    //Ew no, row-major better
    //return row + col * m_nrow;
    return row*this->m_ncol + col;
}

void Matrix::reset_buffer(size_t o_nrow, size_t o_ncol)
{
    if (m_buffer) { delete[] m_buffer; }
    const size_t nelement = nrow * ncol;
    if (nelement) { m_buffer = new double[nelement]; }
    else          { m_buffer = nullptr; }
    m_nrow = o_nrow;
    m_ncol = o_ncol;
}

