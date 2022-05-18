#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>

#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <mkl.h>

namespace py = pybind11;


struct Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    ~Matrix()
    {
        delete m_buffer;
    }

	// get index
    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    double* addr() { return &m_buffer[0]; }


	void reset_buffer(size_t nrow, size_t ncol)
    {
        if (! m_buffer) { delete[] m_buffer;}
		const size_t nelement = nrow * ncol;
        m_buffer = new double[nrow * ncol];//std::vector<double, CustomAllocator<double>>(nelement);//, alloc);
        m_nrow = nrow;
        m_ncol = ncol;
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
        m_elapsed = other.m_elapsed;
        m_nflo = other.m_nflo;

        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }


    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

	size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }

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
 Matrix ex2_multiply_naive(Matrix const & mat1, Matrix const & mat2)
 {
     validate_multiplication(mat1, mat2);

     Matrix ret(mat1.nrow(), mat2.ncol());

     const size_t nrow1 = mat1.nrow();
     const size_t ncol1 = mat1.ncol();
     const size_t nrow2 = mat2.nrow();
     const size_t ncol2 = mat2.ncol();

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

     return ret;
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
		//.def("__eq__", & operator==, py::is_operator())
		.def(py::self == py::self)
		.def("__getitem__", [](Matrix &self, py::tuple tup){
			int row = py::cast<int>(tup[0]);
			int col = py::cast<int>(tup[1]);
			return self.m_buffer[self.index(row, col)];
		})
		.def("__setitem__", [](Matrix &self, py::tuple tup, size_t val){
			int row = py::cast<int>(tup[0]);
			int col = py::cast<int>(tup[1]);
			self.m_buffer[self.index(row, col)] = val;
		})
		.def_property_readonly("array",[](Matrix& m) -> py::array_t<double>{
            return py::array_t<double>(py::memoryview::from_buffer(
                m.m_buffer,
                {m.m_nrow, m.m_ncol},
                {sizeof(double) * m.m_ncol, sizeof(double)}
            ));
        });

	m.def("multiply_naive", & multiply_naive);
	m.def("multiply_mkl", & multiply_mkl);
	m.def("multiply_tile", & multiply_tile);
} 
