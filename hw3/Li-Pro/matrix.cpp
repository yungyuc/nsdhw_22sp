#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#pragma region matrix_decl

struct Matrix
{
public:
    const size_t m_nrow;
    const size_t m_ncolumn;

    Matrix(size_t row, size_t column);
    ~Matrix();

    // in which place is it constexpr, if it's exported?
    constexpr double * operator[] (size_t rowid) { return data + m_ncolumn*rowid; }

    /*double * operator[] (py::tuple rowid);*/  // dealing with `tuple`

private:
    double * data;
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
    : m_nrow(row), m_ncolumn(column)
{
    throw "not yet implemented";
    // check empty matrix
}

Matrix::~Matrix()
{
    // throw "not yet implemented";
}

Matrix
multiply_naive(const Matrix & lhs, const Matrix & rhs)
{
    throw "not yet implemented";
}

Matrix
multiply_tile(const Matrix & lhs, const Matrix & rhs)
{
    throw "not yet implemented";
}

Matrix
multiply_mkl(const Matrix & lhs, const Matrix & rhs)
{
    throw "not yet implemented";
}

#pragma endregion


#pragma region pybind11

namespace py = pybind11;

PYBIND11_MODULE(libmatrix, m) {
    py::class_<Matrix>(m, "_Matrix")
        .def(py::init<size_t, size_t>())
        .def_readonly("nrow", &Matrix::m_nrow)
        .def_readonly("ncol", &Matrix::m_ncolumn)
        /*.def(py::self[size_t()])*/                // operator not supported
        .def("__getitem__", &Matrix::operator[])    // python operator (+ wrapper for args conversion)
    ;

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}

#pragma endregion
