#include <iostream>
#include <vector>
#include <stdexcept>
#include <mkl.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

namespace py = pybind11;

class Matrix
{
    public:
        // basic constructors
        Matrix() : numRows(0), numCols(0) {};

        // custom constructor
        Matrix(size_t srcRows, size_t srcCols);
        
        // copy & move constructor
        Matrix(Matrix const & srcMatrix);
        Matrix(Matrix       && srcMatrix);

        // copy & move assignment operator
        Matrix & operator=(Matrix const & );
        Matrix & operator=(Matrix       &&);

        // Destructor
        virtual ~Matrix() {};

        // Accessors
        size_t sizeRow() const { return numRows; }
        size_t sizeCol() const { return numCols; }
        double const & operator() (size_t i, size_t j) const { return matVal[i*numCols+j]; }
        double       & operator() (size_t i, size_t j)       { return matVal[i*numCols+j]; }
        double* val_addr() { return &matVal[0]; }

        bool operator==(const Matrix &srcMatrix) const
        {
            if (numRows != srcMatrix.sizeRow() || numCols != srcMatrix.sizeCol())
                return false;

            for (size_t i=0 ; i<numRows ; i++)
                for (size_t j=0 ; j <numCols ; j++)
                    if ((*this)(i, j) != srcMatrix(i, j))
                        return false;

            return true;
        }

        double getvalue(std::pair<size_t, size_t> index)
        {
            return (*this)(index.first, index.second);
        }

        void setvalue(std::pair<size_t, size_t> index, const double value)
        {
            (*this)(index.first, index.second) = value;
        }

    private:
        std::vector<double> matVal;
        size_t numRows;
        size_t numCols;
};

Matrix::Matrix(size_t srcRows, size_t srcCols) : numRows(srcRows), numCols(srcCols)
{
    matVal.clear();
    matVal.resize(numRows*numCols, 0);
}

Matrix::Matrix(Matrix const & srcMatrix) : numRows(srcMatrix.numRows), numCols(srcMatrix.numCols)
{
    matVal.resize(numRows, numCols);
    for(size_t i=0 ; i<numRows ; i++)
        for(size_t j=0 ; j<numCols ; j++)
            (*this)(i,j) = srcMatrix(i,j);
}

Matrix::Matrix(Matrix       && srcMatrix) : numRows(srcMatrix.numRows), numCols(srcMatrix.numCols)
{
    this->matVal = std::move(srcMatrix.matVal);
}

Matrix & Matrix::operator=(Matrix const & srcMatrix)
{
    if( this != &srcMatrix )
    {
        this->numRows = srcMatrix.numRows;
        this->numCols = srcMatrix.numCols;
        matVal.resize(numRows, numCols);
        for(size_t i=0 ; i<numRows ; i++)
            for(size_t j=0 ; j<numCols ; j++)
                (*this)(i,j) = srcMatrix(i,j);
    }
    return *this;

}

Matrix & Matrix::operator=(Matrix       && srcMatrix)
{
    if( this != &srcMatrix )
    {
        this->numRows = srcMatrix.numRows;
        this->numCols = srcMatrix.numCols;
        this->matVal = std::move(srcMatrix.matVal);
    }
    return *this;
}

void check_size_match(const Matrix &m1, const Matrix &m2)
{
    if(m1.sizeCol() != m2.sizeRow())
        throw std::out_of_range("Not able to multipy. Make sure the correctness of the size of both matrices.");
}

Matrix multiply_naive(const Matrix &a, const Matrix &b)
{
    check_size_match(a, b);

    Matrix c(a.sizeRow(), b.sizeCol());

    for(size_t i=0 ; i<a.sizeRow() ; i++)
    {        
        for(size_t j=0 ; j<b.sizeCol() ; j++)
        {
            for(size_t k=0 ; k<a.sizeCol() ; k++)
            {
                c(i,j) += a(i,k)*b(k,j);
            }
        }
        
    }

    return c;
}

Matrix multiply_tile(const Matrix &a, const Matrix &b, size_t blocksize)
{
    check_size_match(a, b);

    Matrix c(a.sizeRow(), b.sizeCol());

    for(size_t blocki=0 ; blocki<a.sizeRow() ; blocki+=blocksize)
    {
        size_t i_bound = std::min( blocki+blocksize, a.sizeRow() );
        for(size_t blockj=0 ; blockj<b.sizeCol() ; blockj+=blocksize)
        {
            size_t j_bound = std::min( blockj+blocksize, b.sizeCol() );
            for(size_t blockk=0 ; blockk<a.sizeCol() ; blockk+=blocksize)
            {
                size_t k_bound = std::min( blockk+blocksize, a.sizeCol() );
                for(size_t k=blockk ; k<k_bound ; k++)
                {        
                    for(size_t i=blocki ; i<i_bound ; i++)
                    {
                        for(size_t j=blockj ; j<j_bound ; j++)
                        {
                            c(i,j) += a(i,k)*b(k,j);
                        }
                    }
                }
            }
        }
    }

    return c;
}

Matrix multiply_mkl(Matrix &a, Matrix &b)
{
    check_size_match(a, b);

    Matrix c(a.sizeRow(), b.sizeCol());

    const size_t M = a.sizeRow();
    const size_t N = b.sizeCol();
    const size_t K = a.sizeCol();
    const double alpha=1.0, beta=0.0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                 M, N, K,
                 alpha,
                 a.val_addr(), M,
                 b.val_addr(), N,
                 beta,
                 c.val_addr() ,N);

    return c;

}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix-multiplication function test and performance.";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<size_t, size_t>())
        .def(py::self == py::self)
        .def_property_readonly("nrow", &Matrix::sizeRow)
        .def_property_readonly("ncol", &Matrix::sizeCol)
        .def("__getitem__", &Matrix::getvalue)
        .def("__setitem__", &Matrix::setvalue);
}

