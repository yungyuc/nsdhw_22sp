#include <iostream>
#include <vector>
#include <stdexcept>
#include <mkl.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

using namespace std;
namespace py = pybind11;

class Matrix
{
public:
    // default constructor
    Matrix() : m_rows(0), m_cols(0) {};

    Matrix(size_t row, size_t col) : m_rows(row), m_cols(col)
    {
        m_vec.clear();
        m_vec.resize(row * col, 0);
    }

    // copy constructor
    Matrix(Matrix const &other) : m_rows(other.m_rows), m_cols(other.m_cols)
    {
        m_vec.resize(m_rows, m_cols);
        for (size_t i = 0; i < m_rows; i++)
            for (size_t j = 0; j < m_cols; j++)
                (*this)(i, j) = other(i, j);
    }

    // move constructor
    Matrix(Matrix &&other) : m_rows(other.m_rows), m_cols(other.m_cols)
    {
        this->m_vec = std::move(other.m_vec);
    }

    // copy assignment operator
    Matrix &operator=(Matrix const &other)
    {
        if (this != &other)
        {
            this->m_cols = other.m_cols;
            this->m_rows = other.m_rows;
            m_vec.resize(m_rows, m_cols);
            for (size_t i = 0; i < m_rows; i++)
                for (size_t j = 0; j < m_cols; j++)
                    (*this)(i, j) = other(i, j);
        }
        return *this;
    }

    // move assignment operator
    Matrix &operator=(Matrix &&other)
    {
        if (this != &other)
        {
            this->m_cols = other.m_cols;
            this->m_rows = other.m_rows;
            this->m_vec = std::move(other.m_vec);
        }
        return *this;
    }

    // destructor
    ~Matrix()
    {
        m_vec.clear(); 
        m_vec.shrink_to_fit();
    }

    // getter
    const size_t nrow() const
    {
        return m_rows;
    }
    const size_t ncol() const
    {
        return m_cols;
    }
    double *addr()
    {
        return &m_vec[0];
    }

    // rewrite operator
    double operator () (size_t row, size_t col) const
    {
        return m_vec[m_cols * row + col];
    }
    double &operator () (size_t row, size_t col)
    {
        return m_vec[m_cols * row + col];
    }
    bool operator==(const Matrix &other) const
    {
        if (m_rows != other.nrow() || m_cols != other.ncol())
            return false;

        for (size_t i = 0; i < m_rows; ++i)
            for (size_t j = 0; j < m_cols; ++j)
                if ((*this)(i, j) != other(i, j))
                    return false;

        return true;
    }

    double getitem(std::pair<size_t, size_t> index)
    {
        return (*this)(index.first, index.second);
    }

    void setitem(std::pair<size_t, size_t> index, const double value)
    {
        (*this)(index.first, index.second) = value;
    }

    py::array_t<double> array()
    {
        return py::array_t<double>(
                vector<size_t>{nrow(), ncol()},
                vector<size_t>{sizeof(double) * ncol(), sizeof(double)},
                m_vec.data(),
                py::cast(this)
            );
    }

private:
    vector<double> m_vec;
    size_t m_rows;
    size_t m_cols;
};

void is_validate_matrix(Matrix const &m1, Matrix const &m2)
{
    if (m1.ncol() != m2.nrow())
        throw std::out_of_range("The columns of matrix1 must be equal to the rows of matrix2.");
}

Matrix multiply_naive(Matrix const &m1, Matrix const &m2)
{
    is_validate_matrix(m1, m2);

    Matrix ret(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i)
    {
        for (size_t k = 0; k < m2.ncol(); ++k)
        {
            double v = 0;
            for (size_t j = 0; j < m1.ncol(); ++j)
            {
                v += m1(i, j) * m2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, size_t tsize)
{
    is_validate_matrix(m1, m2);

    Matrix ret(m1.nrow(), m2.ncol());

    for (size_t ti = 0; ti < m1.nrow(); ti += tsize)
    {
        size_t imax = std::min(ti + tsize, m1.nrow());

        for (size_t tj = 0; tj < m2.ncol(); tj += tsize)
        {
            size_t jmax = std::min(tj + tsize, m2.ncol());

            for (size_t tk = 0; tk < m1.ncol(); tk += tsize)
            {
                size_t kmax = std::min(tk + tsize, m1.ncol());

                for (size_t k = tk; k < kmax; ++k)
                {
                    for (size_t i = ti; i < imax; ++i)
                    {
                        for (size_t j = tj; j < jmax; ++j)
                        {
                            ret(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

// Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface
// Ref: https://www.intel.com/content/www/us/en/develop/documentation/mkl-tutorial-c/top/multiplying-matrices-using-dgemm.html#multiplying-matrices-using-dgemm
Matrix multiply_mkl(Matrix &m1, Matrix &m2)
{
    is_validate_matrix(m1, m2);

    Matrix ret(m1.nrow(), m2.ncol());

    const size_t m = m1.nrow();
    const size_t n = m2.ncol();
    const size_t k = m1.ncol();
    double alpha = 1.0, beta = 0.0;

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m,
        n,
        k,
        alpha,
        m1.addr(),
        k,
        m2.addr(),
        n,
        beta,
        ret.addr(),
        n
    );

    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "matrix implementation";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    // Ref: https://pybind11.readthedocs.io/en/stable/advanced/classes.html
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>()) // call the constructor
        .def(py::init<size_t, size_t>())
        .def(py::self == py::self) // operator overloading (==)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", &Matrix::getitem)
        .def("__setitem__", &Matrix::setitem)
        .def_property("array", &Matrix::array, nullptr);
}