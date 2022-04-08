#include <cassert>
#include <cstring>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "mkl/mkl.h"

#pragma region matrix_decl

struct Matrix
{
public:
    size_t const m_nrow;
    size_t const m_ncolumn;

    Matrix(size_t row, size_t column);
    Matrix(Matrix && other);            // used by pybind11 :)
    Matrix(const Matrix &);
    ~Matrix();

    // in which place is it constexpr, if it's exported?
    constexpr double * operator[] (size_t rowid) const { return data + m_ncolumn*rowid; }
    
    // ... and pybind11 does not convert ptr & ref
    constexpr double const & getitem(size_t rowid, size_t colid) const { return (*this)[rowid][colid]; }
    constexpr double & setitem(size_t rowid, size_t colid, double val) const { return (*this)[rowid][colid] = val; }

    /*double * operator[] (py::tuple rowid);*/  // dealing with `tuple`

private:
    double * const data;
};

/**
 * @brief Multiply two matrices, naively without optimization
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @return product matrix
 */
Matrix multiply_naive(const Matrix & lhs, const Matrix & rhs);

/**
 * @brief Multiply two matrices, with matrix tiling
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @return product matrix
 */
Matrix multiply_tile(const Matrix & lhs, const Matrix & rhs);

/**
 * @brief Multiply two matrices, with Intel® MKL
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @return product matrix
 */
Matrix multiply_mkl(const Matrix & lhs, const Matrix & rhs);

#pragma endregion


#pragma region matrix_def

Matrix::Matrix(size_t row, size_t column)
    : m_nrow(row), m_ncolumn(column), data(new double[row*column]{})
{
    // check empty matrix
    if (!row || !column)
    {
        throw std::invalid_argument("can not construct empty Matrix");
    }
}

Matrix::Matrix(const Matrix & other)
    : Matrix(other.m_nrow, other.m_ncolumn)
{
    /*std::copy();*/  // slow
    std::memcpy(data, other.data, sizeof(double)*(m_nrow*m_ncolumn));

    // other.data = nullptr;
}

Matrix::Matrix(Matrix && other)
    : Matrix((const Matrix &) other)
{}

Matrix::~Matrix()
{
    delete[] data;
}

Matrix
multiply_naive(const Matrix & lhs, const Matrix & rhs)
{
    assert( lhs.m_ncolumn == rhs.m_nrow );
    
    Matrix res(lhs.m_nrow, rhs.m_ncolumn);

    for (size_t i = 0; i < lhs.m_nrow; i++)
    {
        for (size_t j = 0; j < rhs.m_ncolumn; j++)
        {
            for (size_t k = 0; k < lhs.m_ncolumn; k++)
            {
                res[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }

    return res;
}

Matrix
multiply_tile(const Matrix & lhs, const Matrix & rhs)
{
    assert( lhs.m_ncolumn == rhs.m_nrow );

    const size_t tile_nrow = 8, tile_ncol = 8;

    size_t N = lhs.m_nrow, M = rhs.m_ncolumn, P = lhs.m_ncolumn;
    Matrix res(N, M);

    for (size_t offi = 0; offi < N; offi += tile_nrow)
    {
        for (size_t offj = 0; offj < M; offj += tile_ncol)
        {
            const size_t lim_i = std::min(offi + tile_nrow, N);
            const size_t lim_j = std::min(offj + tile_ncol, M);

            for (size_t i = offi; i < lim_i; i++)
            {
                for (size_t j = offj; j < lim_j; j++)
                {
                    for (size_t k = 0; k < P; k++)
                    {
                        res[i][j] += lhs[i][k] * rhs[k][j];
                    }
                }
            }
        }
    }

    return res;
}

Matrix
multiply_mkl(const Matrix & lhs, const Matrix & rhs)
{
    size_t N = lhs.m_nrow, M = rhs.m_ncolumn, P = lhs.m_ncolumn;

    // reallocating is relative cheap
    double* lbuf = (double *) mkl_malloc(N*P*sizeof(double), 64);
    double* rbuf = (double *) mkl_malloc(P*M*sizeof(double), 64);
    double* resbuf = (double *) mkl_malloc(N*M*sizeof(double), 64);
    
    if (lbuf == nullptr || rbuf == nullptr || resbuf == nullptr)
    {
        // throw std::bad_alloc("can not allocate extra buffer");
        
        throw std::bad_alloc{};
    }

    std::memcpy(lbuf, lhs[0], sizeof(double)*N*P);
    std::memcpy(rbuf, rhs[0], sizeof(double)*P*M);
    std::memset(resbuf, 0, sizeof(double)*N*M);

    // RES = LBUF * RBUF
    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        N, M, P,
        1.0,  // alpha
        lbuf, P, rbuf, M,
        0.0,  // beta
        resbuf, N
    );

    Matrix res(N, M);
    std::memcpy(res[0], resbuf, sizeof(double)*N*M);
    
    mkl_free(lbuf);
    mkl_free(rbuf);
    mkl_free(resbuf);

    return res;
}

#pragma endregion


#pragma region pybind11

namespace py = pybind11;

PYBIND11_MODULE(libmatrix, m) {
    py::class_<Matrix>(m, "_Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def_readonly("nrow", &Matrix::m_nrow)
        .def_readonly("ncol", &Matrix::m_ncolumn)
        /*.def(py::self[size_t()])*/                    // operator not supported
        /*.def("__getitem__", &Matrix::operator[])*/    // python operator (+ wrapper for args conversion)
        .def("_getitem", &Matrix::getitem)              // helper funcs for __getitem__
        .def("_setitem", &Matrix::setitem)              // helper funcs for __setitem__
    ;

    m.def("_multiply_naive", &multiply_naive);
    m.def("_multiply_tile", &multiply_tile);
    m.def("_multiply_mkl", &multiply_mkl);
}

#pragma endregion
