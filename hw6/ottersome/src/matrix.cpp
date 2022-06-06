#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <algorithm>
#include <math.h>
#ifdef __unix__
#include "mkl.h"
#else
#include "f77blas.h"
#endif
#include "matrix.h"

namespace py = pybind11;



py::array_t<double> Matrix::get_ndarray(){

    //Reference : https://github.com/pybind/pybind11/issues/2271
    return py::array_t<double>(
            {m_nrow,m_ncol},// Shape
            {m_ncol*sizeof(double),sizeof(double)},// Strides
            m_buffer,// Data Pointer
            //py::cast(this)
            py::none() //Lets assume that we wont use the np version when this c++ instance doesnt exist. If not use above
            );

}
Matrix Matrix::multiply_mkl(const Matrix & mat2) const{ 
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
            0.0,
            ret.data(),
            ret.ncol
            );

    return ret;

}
Matrix Matrix::multiply_naive(const Matrix & mat2) const
{
    if (this->m_ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(this->m_nrow, mat2.ncol);

    for (size_t i=0; i<ret.nrow; ++i)
    {
        for (size_t k=0; k<ret.ncol; ++k)
        {
            double v = 0;
            for (size_t j=0; j<this->m_ncol; ++j)
            {
                v += (*this)(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }


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
    //TODO maybe revise this
    std::cerr<<"FAILURE: operator not yet part of implementation!!"<<std::endl;
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
    : m_nrow(other.nrow),m_ncol(other.ncol),nrow(m_nrow),ncol(m_ncol)

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
    std::cerr<<"FAILURE: operator not yet part of implementation!!"<<std::endl;
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
    : nrow(m_nrow),ncol(m_ncol)

{
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
}

Matrix & Matrix::operator=(Matrix && other)
{
    if (this == &other) { return *this; }
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_nrow, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

Matrix::~Matrix()
{
    reset_buffer(0, 0);
}


size_t Matrix::size() const { return m_ncol*m_nrow; }

//Also never call this
double Matrix::buffer(size_t i) const { return m_buffer[i]; }

std::vector<double> Matrix::buffer_vector() const
{
    //I personally never calll this one
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

bool Matrix::operator==(Matrix const & other) const{
    //Compare through memory first maybe ? 
    //We have to time to check if its more effective this way
    if(ncol != other.ncol || nrow != other.nrow)
        return false;

    //Too strict of an approach
    // int n = memcmp(this->data(), other.data(),sizeof(double)*ncol*nrow);
    // return (n==0)? true: false;
    for(size_t row= 0;row<nrow;row++){
        for(size_t col= 0;col<ncol;col++){
            // if(std::abs((*this)(row,col) - other(row,col)) >= EQ_DIFF){
            if((*this)(row,col) != other(row,col)){
                return false;
            }
        }
    }

    return true;
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
    const size_t nelement = o_nrow * o_ncol;
    if (nelement) { m_buffer = new double[nelement]; }
    else          { m_buffer = nullptr; }
    m_nrow = o_nrow;
    m_ncol = o_ncol;
}

