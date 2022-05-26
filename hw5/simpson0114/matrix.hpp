#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <ctime>

#include "/usr/include/mkl/mkl.h"

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
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

    Matrix(Matrix const & other)
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

    Matrix & operator=(Matrix const & other)
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

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    bool operator==(Matrix const & other)
    {
        if (m_nrow != other.nrow() || m_ncol != other.ncol()){
            return false;
        }
        for (size_t i = 0; i < m_nrow; i++){
            for(size_t j = 0; j < m_ncol; j++){
                if (m_buffer[index(i, j)] != other(i, j)) return false;
            }
        }
        return true;
    }

    bool  operator==(Matrix const & other) const
    {
        if (m_nrow != other.nrow() || m_ncol != other.ncol()){
            return false;
        }
        for (size_t i = 0; i < m_nrow; i++){
            for(size_t j = 0; j < m_ncol; j++){
                if (m_buffer[index(i, j)] != other(i, j)) return false;
            }
        }
        return true;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    double *buffer() const {return m_buffer;}

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer+size());
    }

private:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement](); }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};
/*
* Throw an exception if the shapes of the two matrices don't support
* multiplication.
*/
void validate_multiplication(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

void initialize(Matrix & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            if (0 == i%2)
            {
                mat(i, j) = j;
            }
            else
            {
                mat(i, j) = mat.ncol() - 1 - j;
            }
        }
    }
}


Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2){
    validate_multiplication(mat1, mat2);
    Matrix result(mat1.nrow(), mat2.ncol());

    for (int i = 0; i < mat1.nrow(); i++){
        for (int j = 0; j < mat2.ncol(); j++){
            for (int k = 0; k < mat1.ncol(); k++){
                result(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
    }

    return result;
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tileSize){
    validate_multiplication(mat1, mat2);
    Matrix result(mat1.nrow(), mat2.ncol());

    const size_t mrow = mat1.nrow();
    const size_t mcol = mat2.ncol();

    double *mat1Buffer = mat1.buffer();
    double *mat2Buffer = mat2.buffer();
    double *resultBuffer = result.buffer();

    for (int i = 0; i < mrow; i += tileSize){
        for (int j = 0; j < mcol; j += tileSize){
            for (int k = 0; k < mcol; k += tileSize ){
                //int xRange = std::min(i+tileSize, mrow);
                int xRange = ((i+tileSize) < mrow) ? (i+tileSize) : mrow;
                for (int x = i; x < xRange; x++){
                    //int yRange = std::min(j+tileSize, mcol);
                    int yRange = ((j+tileSize) < mcol) ? (j+tileSize) : mcol;
                    for (int y = j; y < yRange; y++){
                        //int zRange = std::min(k+tileSize, mrow);
                        int zRange = ((k+tileSize) < mrow) ? (k+tileSize) : mrow;
                        for (int z = k; z < zRange; z++){
                            //result(x, y) += mat1(x, z) * mat2(z, y);
                            resultBuffer[x*mcol + y] += mat1Buffer[x*mcol + z] * mat2Buffer[z*mcol + y];
                        }
                    }
                }
            }
        }
    }
    return result;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2){
    validate_multiplication(mat1, mat2);
    mkl_set_num_threads(2);
    Matrix result(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.buffer() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.buffer() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , result.buffer() /* double * c */
      , result.ncol() /* const MKL_INT ldc */
    );

    return result;
} 