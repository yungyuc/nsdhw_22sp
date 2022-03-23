#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

//#include "mkl.h"

#include "StopWatch.hpp"

namespace py = pybind11;

struct Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

	// get index
    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
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

    //Matrix & operator=(std::vector<double> const & vec)
	/*Matrix & operator=(py::array_t<double> vec)
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
	*/

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
        m_elapsed = other.m_elapsed;
        m_nflo = other.m_nflo;
        return *this;
    }

    /*Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }*/
	

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

	size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double   elapsed() const { return m_elapsed; }
    double & elapsed()       { return m_elapsed; }

    size_t   nflo() const { return m_nflo; }
    size_t & nflo()       { return m_nflo; }

    double gflops() const { return m_nflo / m_elapsed / 1.e9; }


    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
    double m_elapsed = 0;
    size_t m_nflo = 0; // number of floating-point operations.


};

bool operator== (Matrix const & mat1, Matrix const & mat2)
{
    if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol()))
    {
        return false;
    }

    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t j=0; j<mat1.ncol(); ++j)
        {
            if (mat1(i, j) != mat2(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

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
 * Get the number of floating-point operations.
 */
size_t calc_nflo(Matrix const & mat1, Matrix const & mat2)
{
    return mat1.nrow() * mat1.ncol() * mat2.ncol();
}


/*
 * Direct naive matrix matrix multiplication.
 * rename to multiply_naive
 */
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    StopWatch sw;

    for (size_t i=0; i<nrow1; ++i)
    {
        const size_t base1 = i * ncol1;
        for (size_t k=0; k<ncol2; ++k)
        {
            double v = 0;
            for (size_t j=0; j<ncol1; ++j)
            {
                v += mat1.m_buffer[base1 + j] * mat2.m_buffer[j*ncol2 + k];
            }
            ret.m_buffer[base1 + k] = v;
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{

    Matrix ret(mat1.nrow(), mat2.ncol());

    return ret;

}

//template<size_t N>1
struct Block
{
    //static constexpr const size_t NDIM = N;
	Block(size_t N)
	{
		NDIM = N;
    	m_buffer[NDIM * NDIM];
	}

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block & operator= (double v)
    {
        for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block & operator+= (Block const & other)
    {
        for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);
	
	double* m_buffer = nullptr;
	size_t NDIM;

};

void Block::save(
    Matrix & mat, size_t it, size_t jt
)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat.m_buffer[base_t + j] = m_buffer[base_s + j];
        }
    }
}

//template<size_t N>
struct Tiler
{
    //static constexpr const size_t NDIM = N;
	Tiler(size_t N)
	{
		NDIM = N;
		m_mat1 = Block(N);
		m_mat2 = Block(N);
		m_ret = Block(N);
	}

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

	size_t NDIM;
    Block m_mat1 = NULL; // row-major
    Block m_mat2 = NULL; // column-major
    Block m_ret = NULL; // row-major
};

void Tiler::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
)
{
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat1[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_ret[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat2[j*NDIM + i] = m_ret[base + j];
        }
    }
}

void Tiler::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base1 = i*NDIM;

        for (size_t k=0; k<NDIM; ++k)
        {
            const size_t base2 = k*NDIM;

            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = v;
        }
    }
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t LSIZE)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t tsize = LSIZE / sizeof(double);

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tsize;
    const size_t ntcol1 = ncol1 / tsize;
    const size_t ntrow2 = nrow2 / tsize;
    const size_t ntcol2 = ncol2 / tsize;

    Block value(tsize);
    Tiler tiler(tsize);

    StopWatch sw;

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            value.save(ret, it, kt);
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
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


PYBIND11_MODULE(_matrix, m) {

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
		.def_readonly("nrow", & Matrix::m_nrow)
		.def_readonly("ncol", & Matrix::m_ncol)
		.def("__assign__", & Matrix::operator=, py::is_operator())
		.def("__eq__", & operator==, py::is_operator())
		.def("__getitem__", [](Matrix &self, py::tuple tup){
			int row = py::cast<int>(tup[0]);
			int col = py::cast<int>(tup[1]);
			return self.m_buffer[self.index(row, col)];
		})
		.def("__setitem__", [](Matrix &self, py::tuple tup, size_t val){
			int row = py::cast<int>(tup[0]);
			int col = py::cast<int>(tup[1]);
			self.m_buffer[self.index(row, col)] = val;
		});
		
	m.def("multiply_naive", & multiply_naive);
	m.def("multiply_mkl", & multiply_mkl);
	m.def("multiply_tile", & multiply_tile);
} 

