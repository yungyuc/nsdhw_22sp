#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <math.h>

namespace py = pybind11;

#include "matrix.h"




//TODO can we chagne the return type to be more efficient?

//So we can speed on different size of tiles
//shoudl be multiple of double

//template<size_t ts>
//Matrix Matrix::t_mult(const Matrix & mat2)
//{
    //if (this->ncol() != mat2.nrow())
    //{
        //throw std::out_of_range(
            //"the number of first matrix column "
            //"differs from that of second matrix row");
    //}

    ////Create matrix to return 
    //Matrix retMat(this->nrow(), mat2.ncol());
    //retMat.zero_out();


    ////Get size of tiles:
    ////TODO handle the edge cases
    //size_t num_rt1 = std::ceil((sizeof(double)*m_nrow)/ts);
    //size_t num_ct1 = std::ceil((sizeof(double)*m_ncol)/ts);
    ////size_t num_rt2 = (sizeof(double)*mat2.nrow())/ts;
    //size_t num_ct2 = std::ceil((sizeof(double)*mat2.ncol())/ts);

    //size_t curCellHeight =0;
    //size_t curCellWidth = 0;
    //size_t adder = 0;
    //size_t operation_num= 0;
    ////Go around Cells in increments of their respective size
    //for(size_t rtile = 0;rtile<retMat.nrow();rtile+=ts){
        //for(size_t ctile = 0;ctile< retMat.ncol();ctile+=ts){

            ////Actually do cell by cell dot product
            //curCellHeight =  std::min(ts, retMat.nrow()- rtile);
            //curCellWidth = std::min(ts, retMat.ncol()- ctile);
            //operation_num = std::min(ts, (this)->ncol()-ctile);
            ////For each element of the dot product
            ////Size of 
            //adder = 0;
            //for(size_t tile_row = 0;tile_row<curCellHeight;tile_row++){
                //for(size_t tile_col = 0;tile_col<curCellWidth;tile_col++){
                    //for(size_t elem = 0; elem< operation_num;elem++){
                        //retMat(rtile,ctile) = (*this)(rtile+tile_row,ctile+tile_col+elem)
                                        //* mat2(rtile+tile_row+elem,ctile+tile_col);
                        ////retMat(rtile,ctile) = (*this)(rtile+tile_row,ctile+tile_col) + mat2(ctile+tile_col,rtile+tile_row);
                    //}
                //}
                //adder = 0;

            //}

        //}

    //}


    //return retMat;
//}


Matrix Matrix::n_mult(const Matrix & mat2)
{
    if (this->ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(this->nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<this->ncol(); ++j)
            {
                v += (*this)(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

Matrix::Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

Matrix::Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
    : m_nrow(nrow), m_ncol(ncol)
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
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = vec[k];
            ++k;
        }
    }

    return *this;
}

Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(other.m_nrow, other.m_ncol);
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
}

Matrix & Matrix::operator=(Matrix const & other)
{
    if (this == &other) { return *this; }
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
    }
    for (size_t i=0; i<m_nrow; ++i)
    {
        for (size_t j=0; j<m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}

Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
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
    std::swap(m_ncol, other.m_ncol);
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

size_t Matrix::nrow() const { return m_nrow; }
size_t Matrix::ncol() const { return m_ncol; }
size_t Matrix::size() const { return m_ncol*m_nrow; }

double Matrix::buffer(size_t i) const { return m_buffer[i]; }
std::vector<double> Matrix::buffer_vector() const
{
    return std::vector<double>(m_buffer, m_buffer+size());
}


double * Matrix::data(){
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

void Matrix::reset_buffer(size_t nrow, size_t ncol)
{
    if (m_buffer) { delete[] m_buffer; }
    const size_t nelement = nrow * ncol;
    if (nelement) { m_buffer = new double[nelement]; }
    else          { m_buffer = nullptr; }
    m_nrow = nrow;
    m_ncol = ncol;
}

