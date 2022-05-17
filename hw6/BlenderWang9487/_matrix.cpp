#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <mkl_cblas.h>
#include <stdexcept>
#include <vector>

namespace py = pybind11;
class Matrix
{

public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
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

    Matrix(Matrix &&other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
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

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double operator()(size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }
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
    const double *buffer() const { return m_buffer; }
    double *buffer() { return m_buffer; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer + size());
    }
private:
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
            // array = py::array_t<double>(py::buffer_info(
            //     m_buffer,                               /* Pointer to buffer */
            //     sizeof(double),                          /* Size of one scalar */
            //     py::format_descriptor<double>::format(), /* Python struct-style format descriptor */
            //     2,                                      /* Number of dimensions */
            //     { m_nrow, m_ncol },                 /* Buffer dimensions */
            //     { sizeof(double) * m_ncol,             /* Strides (in bytes) for each index */
            //     sizeof(double) }
            // ));
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

Matrix multiply_naive(const Matrix &m1, const Matrix &m2)
{
    if (m1.ncol() != m2.nrow())
    {
        throw std::out_of_range(
            "number of m1's col and m2's row mismatch");
    }
    size_t r = m1.nrow();
    size_t c = m2.ncol();
    Matrix ret(r, c);
    for (size_t i = 0; i < r; i++)
        for (size_t j = 0; j < c; j++)
        {
            double sum = 0.0;
            for (size_t k = 0; k < m1.ncol(); k++)
                sum += m1(i, k) * m2(k, j);
            ret(i, j) = sum;
        }
    return ret;
}

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t tsize)
{
    if (m1.ncol() != m2.nrow())
    {
        throw std::out_of_range(
            "number of m1's col and m2's row mismatch"
        );
    }
    size_t r = m1.nrow();
    size_t c = m2.ncol();
    size_t k = m1.ncol();
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
                    for (size_t col = c_tile_start; col < c_tile_end; ++col)
                    {
                        size_t index = row*c + col;
                        for (size_t kth = k_tile_start; kth < k_tile_end; ++kth)
                            ret.buffer(index) += m1(row, kth) * m2(kth, col);
                    }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix& m1,const Matrix& m2)
{
    if(m1.ncol() != m2.nrow())
    {
        throw std::out_of_range(
            "number of m1's col and m2's row mismatch"
        );
    }
    int m = m1.nrow();
    int n = m2.ncol();
    int k = m1.ncol();
    Matrix ret(m,n);
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m,
        n,
        k,
        1.0,
        m1.buffer(),
        k,
        m2.buffer(),
        n,
        0.0,
        ret.buffer(),
        n
    );
    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t,size_t>())
        .def(py::init<size_t,size_t,std::vector<double> const&>())
        .def("__getitem__",
            [](const Matrix& mat,std::pair<size_t,size_t> index)
            {
                return mat(index.first,index.second);
            }
        )
        .def("__setitem__",
            [](Matrix& mat,std::pair<size_t,size_t> index,double value)
            {
                mat(index.first,index.second) = value;
            }
        )
        .def("__eq__",&Matrix::operator==)
        .def("__ne__",
            [](const Matrix& lhs,const Matrix& rhs)
            {
                return !(lhs == rhs);
            }
        )
        .def_property_readonly("array",[](Matrix& mat) -> py::array_t<double>{
            return py::array_t<double>(py::memoryview::from_buffer(
                mat.buffer(),
                {mat.nrow(), mat.ncol()},
                {sizeof(double) * mat.ncol(), sizeof(double)}
            ));
        })
        .def_property_readonly("nrow",&Matrix::nrow)
        .def_property_readonly("ncol",&Matrix::ncol)
        ;
    m
    .def("multiply_naive",&multiply_naive,
        py::arg("m1"),
        py::arg("m2"))
    .def("multiply_mkl",&multiply_mkl,
        py::arg("m1"),
        py::arg("m2"))
    ;
}