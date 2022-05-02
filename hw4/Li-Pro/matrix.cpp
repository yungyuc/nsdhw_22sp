#include <cassert>
#include <cstring>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "mkl/mkl.h"

#pragma region matrix_decl

constexpr double EPS = 1e-6;

template<typename Tp>
struct CountableAllocator
{
    using value_type = Tp;
    using pointer = value_type*;
    using size_type = std::size_t;

    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n);

    constexpr static std::size_t bytes() { return allocated() - deallocated(); }
    constexpr static std::size_t allocated() { return allocated_size; }
    constexpr static std::size_t deallocated() { return deallocated_size; }

private:
    static size_type allocated_size;
    static size_type deallocated_size;
};

struct Matrix
{
public:
    using allocator_type = CountableAllocator<double>;
    using buffer_type = std::vector<double, allocator_type>;

    size_t const m_nrow;
    size_t const m_ncolumn;

    Matrix(size_t row, size_t column);
    Matrix(Matrix && other);            // used by pybind11 :)
    Matrix(const Matrix &) = delete;
    ~Matrix();

    // in which place is it constexpr, if it's exported?
    constexpr double * operator[] (size_t rowid) const
    {
        // data is contiguos
        return (double*) data.data() + m_ncolumn*rowid;
    }
    
    constexpr bool operator== (const Matrix & other) const
    {
        // return m_nrow == other.m_nrow
        //     && m_ncolumn == other.m_ncolumn
        //     && std::memcmp(data, other.data, memsize()) == 0;

        if (!(m_nrow == other.m_nrow && m_ncolumn == other.m_ncolumn))
        {
            return false;
        }
        
        for (size_t i = 0; i < m_nrow; i++)
        {
            for (size_t j = 0; j < m_ncolumn; j++)
            {
                if (!(std::abs((*this)[i][j] - other[i][j]) < EPS))
                {
                    return false;
                }
            }
        }

        return true;
    }
    
    // ... and pybind11 does not convert ptr & ref
    constexpr double const & getitem(size_t rowid, size_t colid) const { return (*this)[rowid][colid]; }
    constexpr double & setitem(size_t rowid, size_t colid, double val) const { return (*this)[rowid][colid] = val; }

    /*double * operator[] (py::tuple rowid);*/  // dealing with `tuple`

private:
    constexpr size_t memsize() const { return sizeof(double) * m_nrow * m_ncolumn; }

    buffer_type data;
};

/**
 * @brief Multiply two matrices, naively without optimization.
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @return product matrix
 */
Matrix multiply_naive(const Matrix & lhs, const Matrix & rhs);

/**
 * @brief Multiply two matrices, with matrix tiling.
 * The method is not exported.
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @param tile_nrow tile size for row
 * @param tile_ncol tile size for column
 * @return Matrix product matrix
 */
Matrix multiply_tile(const Matrix & lhs, const Matrix & rhs, const size_t tile_nrow, const size_t tile_ncol, const size_t tile_nenum);

/**
 * @brief Multiply two matrices, with matrix tiling.
 * 
 * @param lhs multiplier matrix
 * @param rhs multiplicand matrix
 * @param tile_size tile size for row & column
 * @return Matrix product matrix
 */
Matrix multiply_tile(const Matrix & lhs, const Matrix & rhs, const size_t tile_size);

/**
 * @brief Multiply two matrices, with matrix tiling (tile size = 8).
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

template<typename Tp>
typename CountableAllocator<Tp>::size_type
CountableAllocator<Tp>::allocated_size = 0;

template<typename Tp>
typename CountableAllocator<Tp>::size_type
CountableAllocator<Tp>::deallocated_size = 0;

template<typename Tp>
typename CountableAllocator<Tp>::pointer
CountableAllocator<Tp>::allocate(CountableAllocator<Tp>::size_type n)
{
    allocated_size += sizeof(Tp[n]);
    return new Tp[n];
}

template<typename Tp>
void
CountableAllocator<Tp>::deallocate(
    CountableAllocator<Tp>::pointer p, CountableAllocator<Tp>::size_type n
){
    deallocated_size += sizeof(Tp[n]);
    delete[] p;
}

Matrix::Matrix(size_t row, size_t column)
    : m_nrow(row), m_ncolumn(column), data(row * column)
{
    // check empty matrix
    if (!row || !column)
    {
        throw std::invalid_argument("can not construct empty Matrix");
    }
}

Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncolumn(other.m_ncolumn), data(std::move(other.data))
{}

Matrix::~Matrix()
{}

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
multiply_tile(const Matrix & lhs, const Matrix & rhs, const size_t tile_nrow, const size_t tile_ncol, const size_t tile_nenum)
{
    assert( lhs.m_ncolumn == rhs.m_nrow );

    size_t N = lhs.m_nrow, M = rhs.m_ncolumn, P = lhs.m_ncolumn;
    Matrix res(N, M);

    for (size_t offi = 0; offi < N; offi += tile_nrow)
    {
        for (size_t offj = 0; offj < M; offj += tile_ncol)
        {
            for (size_t offk = 0; offk < P; offk += tile_nenum)
            {
                const size_t lim_i = std::min(offi + tile_nrow, N);
                const size_t lim_j = std::min(offj + tile_ncol, M);
                const size_t lim_k = std::min(offk + tile_nenum, P);

                for (size_t i = offi; i < lim_i; i++)
                {
                    for (size_t j = offj; j < lim_j; j++)
                    {
                        for (size_t k = offk; k < lim_k; k++)
                        {
                            res[i][j] += lhs[i][k] * rhs[k][j];
                        }
                    }
                }
            }
        }
    }

    return res;
}

Matrix
multiply_tile(const Matrix & lhs, const Matrix & rhs, const size_t tile_size)
{
    if (!tile_size)
    {
        return multiply_naive(lhs, rhs);
    }

    const size_t tile_nrow = tile_size, tile_ncol = tile_size, tile_nenum = tile_size;
    return multiply_tile(lhs, rhs, tile_nrow, tile_ncol, tile_nenum);
}

Matrix
multiply_tile(const Matrix & lhs, const Matrix & rhs)
{
    const size_t tile_size = 8;
    return multiply_tile(lhs, rhs, tile_size);
}

Matrix
multiply_mkl(const Matrix & lhs, const Matrix & rhs)
{
    size_t N = lhs.m_nrow, M = rhs.m_ncolumn, P = lhs.m_ncolumn;

    // reallocating is relative cheap
    // We DO NOT use Alloc here, for speed
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
        // .def(py::init<const Matrix &>())
        .def(py::init([](Matrix & other) { return new Matrix(std::move(other)); }))
        .def_readonly("nrow", &Matrix::m_nrow)
        .def_readonly("ncol", &Matrix::m_ncolumn)
        /*.def(py::self[size_t()])*/                    // operator not supported
        /*.def("__getitem__", &Matrix::operator[])*/    // python operator (+ wrapper for args conversion)
        .def("_getitem", &Matrix::getitem)              // helper funcs for __getitem__
        .def("_setitem", &Matrix::setitem)              // helper funcs for __setitem__
        .def(py::self == py::self)
    ;

    m.attr("EPS") = EPS;

    m.def("_multiply_naive", &multiply_naive);
    m.def("_multiply_tile", static_cast<Matrix (*)(const Matrix & lhs, const Matrix & rhs)>(&multiply_tile));
    m.def("_multiply_tile", static_cast<Matrix (*)(const Matrix & lhs, const Matrix & rhs, const size_t tile_size)>(&multiply_tile));
    m.def("_multiply_mkl", &multiply_mkl);

    m.def("bytes", &CountableAllocator<double>::bytes);
    m.def("allocated", &CountableAllocator<double>::allocated);
    m.def("deallocated", &CountableAllocator<double>::deallocated);
}

#pragma endregion
