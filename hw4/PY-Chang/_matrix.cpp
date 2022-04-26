#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <ctime>

#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "/usr/include/mkl/mkl.h"

namespace py = pybind11;

struct ByteCounterImpl
{

    std::atomic_size_t allocated{0};
    std::atomic_size_t deallocated{0};
    std::atomic_size_t refcount{0};

}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */
template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

// template <class T, class U>
// bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
// {
//     return a.counter == b.counter;
// }

// template <class T, class U>
// bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
// {
//     return !(a == b);
// }


static MyAllocator<double> alloc;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol),  m_buffer(alloc)
    {
        reset_buffer(nrow, ncol);
    }

    // Matrix(size_t nrow, size_t ncol, std::vector<double, MyAllocator<double>> const & vec)
    //   : m_nrow(nrow), m_ncol(ncol)
    // {
    //     reset_buffer(nrow, ncol);
    //     (*this) = vec;
    // }

    Matrix & operator=(std::vector<double, MyAllocator<double>> const & vec)
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
                //(*this)(i,j) = vec[k];
                (*this) = vec;
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
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
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    bool operator==(Matrix const & other)
    {
        if (m_nrow != other.nrow() || m_ncol != other.ncol()){
            return false;
        }
        for (size_t i = 0; i < m_nrow; i++){
            for(size_t j = 0; j < m_ncol; j++){
                if (m_buffer[index(i, j)] != other(i, j)) return false;
            }
        }
        return true;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
        
    }

    double operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    //std::vector<double> buffer() const {return m_buffer;}
    double *buffer() {return m_buffer.data();}
    const double *buffer() const { return m_buffer.data(); }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
//  std::vector<double> buffer_vector() const
//  {
//      return std::vector<double>(m_buffer, m_buffer+size());
//  }

private:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        m_buffer.clear();
        const size_t nelement = nrow * ncol;
        m_buffer.resize(nelement, 0.0);
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    //double * m_buffer = nullptr;
    // std::vector<double> m_buffer;

    // MyAllocator<double> alloc;
    std::vector<double, MyAllocator<double>> m_buffer;


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

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2){
    validate_multiplication(mat1, mat2);
    Matrix result(mat1.nrow(), mat2.ncol());
    
    for (int i = 0; i < mat1.nrow(); i++){
        for (int j = 0; j < mat2.ncol(); j++){
            for (int k = 0; k < mat1.ncol(); k++){
                result(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
    }
    
    return result;
}

Matrix multiple_tile(Matrix const & mat1, Matrix const & mat2, size_t tileSize){
    validate_multiplication(mat1, mat2);
    Matrix result(mat1.nrow(), mat2.ncol());

    const size_t mrow = mat1.nrow();
    const size_t mcol = mat2.ncol();

    // double *mat1Buffer = mat1.buffer();
    // double *mat2Buffer = mat2.buffer();
    // double *resultBuffer = result.buffer();

    for (int i = 0; i < mrow; i += tileSize){
        for (int j = 0; j < mcol; j += tileSize){
            for (int k = 0; k < mcol; k += tileSize ){
                //int xRange = std::min(i+tileSize, mrow);
                int xRange = ((i+tileSize) < mrow) ? (i+tileSize) : mrow;
                for (int x = i; x < xRange; x++){
                    //int yRange = std::min(j+tileSize, mcol);
                    int yRange = ((j+tileSize) < mcol) ? (j+tileSize) : mcol;
                    for (int y = j; y < yRange; y++){
                        //int zRange = std::min(k+tileSize, mrow);
                        int zRange = ((k+tileSize) < mrow) ? (k+tileSize) : mrow;
                        for (int z = k; z < zRange; z++){
                            result(x, y) += mat1(x, z) * mat2(z, y);
                            //resultBuffer[x*mcol + y] += mat1Buffer[x*mcol + z] * mat2Buffer[z*mcol + y];
                        }
                    }
                }
            }
        }
    }
    return result;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2){
    validate_multiplication(mat1, mat2);
    mkl_set_num_threads(2);
    Matrix result(mat1.nrow(), mat2.ncol());
    // double mat1_buffer_ptr[mat1.nrow()*mat1.ncol()];
    // double mat2_buffer_ptr[mat2.nrow()*mat2.ncol()];
    // double result_buffer_ptr[result.nrow()*result.ncol()];

    // std::copy(mat1.buffer().begin(), mat1.buffer().end(), mat1_buffer_ptr);
    // std::copy(mat2.buffer().begin(), mat2.buffer().end(), mat2_buffer_ptr);
    //std::copy(result.buffer().begin(), result.buffer().end(), result_buffer_ptr);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.buffer() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.buffer() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , result.buffer() /* double * c */
      , result.ncol() /* const MKL_INT ldc */
    );

    // for (int i=0; i< result.nrow()*result.ncol(); i++){
    //     std::cout<<result_buffer_ptr[i];
    // }

    // for (int i=0; i< mat2.nrow()*mat2.ncol(); i++){
    //     std::cout<<mat2_buffer_ptr[i];
    // }
    
    //result.buffer().assign(result_buffer_ptr, result_buffer_ptr);
    return result;
}

std::size_t bytes() { return alloc.counter.bytes(); }
std::size_t allocated() { return alloc.counter.allocated(); }
std::size_t deallocated() { return alloc.counter.deallocated();}

int main(int argc, char *argv[]){
    constexpr size_t size = 128;
    Matrix a(size, size);
    initialize(a);
    Matrix b = a;

    clock_t start, end;
    start = clock();
    Matrix mklAns = multiply_mkl(a, b);
    end = clock();
    double dur = (double)(end - start);
    std::cout<<"mtk done "<<dur/CLOCKS_PER_SEC<<std::endl;

    Matrix naiveAns = multiply_naive(a, b);
    std::cout<<"naive done"<<std::endl;

    Matrix tileAns = multiple_tile(a, b ,32);
    std::cout<<"tile done"<<std::endl;

    if (naiveAns == tileAns)
        std::cout<<"equal"<<std::endl;
}

PYBIND11_MODULE(_matrix, m){
    m.def("initialize", &initialize);
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiple_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(
          "__getitem__",
          [](Matrix &mat, std::pair<size_t, size_t> p) {
            return mat(p.first, p.second);
        })
        .def(
          "__setitem__",
          [](Matrix &mat, std::pair<size_t, size_t> p, double val) {
            mat(p.first, p.second) = val;
        })
        .def(
          "__setitem__",
          [](Matrix mat1, Matrix mat2) {
            mat1 = mat2;
        })
        .def("__eq__", &Matrix::operator==)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);
}