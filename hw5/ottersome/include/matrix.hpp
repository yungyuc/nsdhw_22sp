#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 
#include <chrono>
#include "mkl.h"
using namespace std::chrono;
#define EQ_DIFF 1.0e-5

namespace py = pybind11;
class Matrix {
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

public:


    Matrix(size_t nrow, size_t ncol);
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);

    Matrix & operator=(std::vector<double> const & vec);
    Matrix(Matrix const & other);
    Matrix & operator=(Matrix const & other);
    Matrix(Matrix && other);
    Matrix & operator=(Matrix && other);
    ~Matrix();

    double const & operator() (size_t row, size_t col) const{
        return m_buffer[row*m_ncol + col];
    };
    double & operator() (size_t row, size_t col){
        return m_buffer[row*m_ncol + col];
    }

    bool operator==(const Matrix & other) const;

    //double get(size_t row, size_t col) const;
    double & get(size_t row, size_t col) ;

    //For convenience
    const size_t  & nrow;
    const size_t  & ncol;

    //For python members
    size_t get_nrow() const{ return m_nrow;}
    size_t get_ncol() const { return m_ncol;}

    size_t size() const;
    double buffer(size_t i) const; std::vector<double> buffer_vector() const;

    //Arithmetic Operations
    Matrix multiply_naive(const Matrix & mat2) const;
    Matrix multiply_mkl(const Matrix & mat2) const;
    
    //template<size_t ts_bytes>
    Matrix multiply_tile(const Matrix & mat2, size_t ts) const{

        // auto start = high_resolution_clock::now();
        if (this->ncol != mat2.nrow)
        {
            throw std::out_of_range(
                    "the number of first matrix column "
                    "differs from that of second matrix row");
        }

        //Create matrix to return and zero out its elements
        Matrix retMat(this->nrow, mat2.ncol);
        retMat.zero_out();

        size_t tile_vbound =0, tile_hbound=0,op_bound=0;
        // auto b4_loop = high_resolution_clock::now();

        // auto aft_loop = high_resolution_clock::now();
        
        //Go around Cells in increments of their respective size
        for(size_t rtile = 0; rtile < this->nrow;rtile+=ts)
        {
            tile_vbound =  std::min(ts+rtile, retMat.nrow);
            for(size_t t_col = 0;t_col< mat2.ncol;t_col+=ts)
            {
                tile_hbound = std::min(ts+t_col, mat2.ncol);
                //For every vertical tile in matrix 2(of the ctil tile column)
                for(size_t vtile=0;vtile<mat2.nrow;vtile+=ts)
                {
                    //Actually do cell by cell dot product
                    op_bound = std::min(ts+vtile, (this)->ncol);
                    for(size_t elem = vtile; elem< op_bound;elem++)
                    {
                        for(size_t row = rtile;row<tile_vbound;row++)
                        {
                            for(size_t col = t_col;col<tile_hbound;col++)
                            {
                                // rtr = rtile+row;
                                retMat(row,col)  += (*this)(row,elem)
                                    * mat2(elem,col);
                            }
                        }
                    }
                }

            }

        }

        // auto prep_duration = duration_cast<milliseconds>(b4_loop-start);
        // auto loop_duration = duration_cast<milliseconds>(aft_loop-b4_loop);
        // auto tot_duration = duration_cast<milliseconds>(aft_loop-start);
        // std::cout << "Execution Times:\n"
        //     <<"\tTotalDuration"<<tot_duration.count()<<"\n"
        //     <<"\tprep_duration"<<prep_duration.count()<<"\n"
        //     <<"\tloop_duration"<<loop_duration.count()<<"\n"
        //     <<std::endl;

        return retMat;

    }

    //Return pointer to Data
    double * data();
    double * data() const;

    //Helpers
    void zero_out();
    
    //Info
    void print_vals();

};

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
  
    //if(m_nrow > 100 or m_ncol){
        //for(size_t r=0;r< m_nrow;r++){
            //for(size_t c=0;c< m_ncol;c++){
                //py::print((*this)(r,c));
            //}
            //py::print("\n");
        //}
    //}else{
        //py::print("Matrix too big to print");
    //}
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

Matrix multiply_naive(const Matrix &m1, const Matrix &m2){
    return m1.multiply_naive(m2);
}
Matrix multiply_tile(const Matrix &m1, const Matrix &m2,size_t tile_size){
    return m1.multiply_tile(m2, tile_size);
}
Matrix multiply_mkl(Matrix &m1, const Matrix &m2){
    return m1.multiply_mkl(m2);
}
