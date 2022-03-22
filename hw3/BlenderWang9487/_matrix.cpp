// #include <pybind11/pybind11.h>
// #include <mkl_lapack.h>
#include <stdexcept>
#include <vector>

// namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol, bool column_major)
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix
    (
        size_t nrow, size_t ncol, bool column_major
      , std::vector<double> const & vec
    )
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
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
      , m_column_major(other.m_column_major)
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
      , m_column_major(other.m_column_major)
    {
        reset_buffer(0, 0);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        m_column_major = other.m_column_major;
        return *this;
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

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer+size());
    }

    double * data() const { return m_buffer; }
private:

    size_t index(size_t row, size_t col) const
    {
        if (m_column_major) { return row          + col * m_nrow; }
        else                { return row * m_ncol + col         ; }
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    bool m_column_major = false;
    double * m_buffer = nullptr;

};

Matrix multiply_naive(const Matrix& m1,const Matrix& m2)
{
    if(m1.ncol() != m2.nrow())
    {
        throw std::out_of_range(
            "number of m1's col and m2's row mismatch"
        );
    }
    size_t r = m1.nrow();
    size_t c = m2.ncol();
    Matrix ret(r, c, false);
    for(size_t i=0;i < r;i++)
        for(size_t j=0;j < c;j++)
        {
            double sum = 0.0;
            for(size_t k=0;k < m1.ncol();k++)
                sum += m1(i,k) * m2(k,j);
            ret(i,j) = sum;
        }
    return ret;
}
