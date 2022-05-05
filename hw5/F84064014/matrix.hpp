#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <mkl_cblas.h>
#include <stdexcept>
#include <vector>
#include <mkl.h>

namespace py = pybind11;
class Matrix
{

public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    Matrix &operator=(std::vector<double> const &vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const &other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    Matrix &operator=(Matrix const &other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

	Matrix &operator=(Matrix &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }


    Matrix(Matrix &&other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }


    double operator()(size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double &operator()(size_t row, size_t col)      { return m_buffer[index(row, col)]; }
    
	bool operator==(const Matrix& rhs) const
    {
        size_t r = rhs.m_nrow;
        size_t c = rhs.m_ncol;
        if(m_nrow != r || m_ncol != c)
            return false;
        for(size_t i = 0;i < r*c;i++)
            if(m_buffer[i] != rhs.m_buffer[i])
                return false;
        return true;
    }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    double& buffer(size_t i) { return m_buffer[i]; }
    std::vector<double> buffer_vector() const{return std::vector<double>(m_buffer, m_buffer + size());}

	// get index
    size_t index(size_t row, size_t col) const
    {
        return row*m_ncol + col;  // *** Row major ***
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement)
        {
            m_buffer = new double[nelement]();
        }
        else
        {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
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

/*
* row
*/

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t k = 0; k < mat2.ncol(); ++k)
        {
            double v = 0; 
            for (size_t j = 0; j < mat1.ncol(); ++j)
            {
                v += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
}


Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t tsize)
{


     validate_multiplication(mat1, mat2);

    size_t r = mat1.nrow();
    size_t c = mat2.ncol();
    size_t k = mat1.ncol();
    Matrix ret(r, c);
    for (size_t r_tile_start = 0; r_tile_start < r; r_tile_start += tsize)
    {
        size_t r_tile_end = std::min(r_tile_start + tsize, r);
        for (size_t c_tile_start = 0; c_tile_start < c; c_tile_start += tsize)
        {
            size_t c_tile_end = std::min(c_tile_start + tsize, c);
            for (size_t k_tile_start = 0; k_tile_start < k; k_tile_start += tsize)
            {
                size_t k_tile_end = std::min(k_tile_start + tsize, k);
                for (size_t row = r_tile_start; row < r_tile_end; ++row)
                {
                    size_t base1 = row*r;
                    for (size_t col = c_tile_start; col < c_tile_end; ++col)
                    {
                        size_t index = row*c + col;
                        for (size_t kth = k_tile_start; kth < k_tile_end; ++kth)
                            ret.m_buffer[index] += mat1.m_buffer[base1+kth] * mat2(kth, col);
                    }
                }
            }
        }
    }
    return ret;
}


/*
 * Use MKL for the matrix matrix multiplication.
 */

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());


    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;

}

