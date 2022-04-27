#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>
#include <string>
#include <iostream>

#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

struct ByteCounterImpl{
    std::atomic_size_t allocated{0};
    std::atomic_size_t deallocated{0};
}; 

class ByteCounter{

public:
    
    ByteCounter(): m_impl(new ByteCounterImpl){}
    void swap(ByteCounter & other);
    void increase(std::size_t amount);
    void decrease(std::size_t amount);
    std::size_t bytes() const;
    std::size_t allocated() const;
    std::size_t deallocated() const;
    
private:
    ByteCounterImpl * m_impl;
};

void ByteCounter::swap(ByteCounter & other){
        std::swap(m_impl, other.m_impl);
}

void ByteCounter::increase(std::size_t amount){
        m_impl->allocated += amount;
}

void ByteCounter::decrease(std::size_t amount){
        m_impl->deallocated += amount;
}

std::size_t ByteCounter::bytes() const { return m_impl->allocated - m_impl->deallocated; }
std::size_t ByteCounter::allocated() const { return m_impl->allocated; }
std::size_t ByteCounter::deallocated() const { return m_impl->deallocated; }

template <class T>
struct Allocator
{
    using value_type = T;

    Allocator() = default;

    template <class U> constexpr
    Allocator(const Allocator<U> & other) noexcept
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
};


 template <class T, class U>
 bool operator==(const Allocator<T> & a, const Allocator<U> & b)
 {
     return (a == b);
 }

 template <class T, class U>
 bool operator!=(const Allocator<T> & a, const Allocator<U> & b)
 {
     return !(a == b);
 } 


template <class T>
std::ostream & operator << (std::ostream & out, const Allocator<T> & alloc)
{
    out << "allocator: bytes = " << alloc.counter.bytes();
    out << " allocated = " << alloc.counter.allocated();
    out << " deallocated = " << alloc.counter.deallocated();
    return out;
}


Allocator<double> alloc;

size_t bytes(){
     return alloc.counter.bytes();
}

size_t allocated(){
     return alloc.counter.allocated();
}

size_t deallocated(){
     return alloc.counter.deallocated();
}

class Matrix{
 public:
    Matrix(size_t row, size_t col);
    ~Matrix();
    Matrix(Matrix &&other);
     // getter
    const size_t nrow() const;
    const size_t ncol() const;
    double *addr();
    
     // rewrite operator
    double operator()(size_t row, size_t col) const;
    double &operator()(size_t row, size_t col);
    bool operator==(const Matrix &other) const;

    double getitem(std::pair<size_t, size_t> index);
    void setitem(std::pair<size_t, size_t> index, const double value);
    
    std::string str() const;

private:
    std::vector<double, Allocator<double>> m_vec;
    size_t m_rows;
    size_t m_cols;
};


Matrix::Matrix(size_t row, size_t col) : m_rows(row), m_cols(col), m_vec(alloc){
    m_vec.clear();
    m_vec.resize(row * col, 0);
}

Matrix::~Matrix(){
    m_vec.clear();
    m_vec.shrink_to_fit();
}
     // move constructor
Matrix::Matrix(Matrix &&other) : m_vec(std::move(other.m_vec))
     {
         std::swap(this->m_rows, other.m_rows);
         std::swap(this->m_cols, other.m_cols);
     }
     

const size_t Matrix::nrow() const{ return m_rows;}
const size_t Matrix::ncol() const{ return m_cols;} 

double *Matrix::addr(){ return &m_vec[0];}
 
double Matrix::operator()(size_t row, size_t col) const{
         return m_vec[m_cols * row + col];
}

double& Matrix::operator()(size_t row, size_t col){
         return m_vec[m_cols * row + col];
}

bool Matrix::operator==(const Matrix &other) const{
    if (m_rows != other.nrow() || m_cols != other.ncol()) return false;

    for (size_t i = 0; i < m_rows; ++i){
        for (size_t j = 0; j < m_cols; ++j){
            if ((*this)(i, j) != other(i, j)) return false;
        }
    }
    return true;
}

double Matrix::getitem(std::pair<size_t, size_t> index){
        return (*this)(index.first, index.second);
}

void Matrix::setitem(std::pair<size_t, size_t> index, const double value){
        (*this)(index.first, index.second) = value;
}
 
std::string Matrix::str() const{
        std::string s = "";
        for (size_t i = 0; i < m_rows; ++i){
            for (size_t j = 0; j < m_cols; ++j)
                s += std::to_string(m_vec[i * m_cols + j]) + ' ';
            s += '\n';
        }
        return s;
}

void is_validate_matrix(Matrix const &m1, Matrix const &m2){
    if (m1.ncol() != m2.nrow())
        throw std::out_of_range("The columns of matrix1 must be equal to the rows of matrix2.");
}

Matrix multiply_naive(Matrix const &m1, Matrix const &m2){
    is_validate_matrix(m1, m2);

    Matrix ret(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i){
        for (size_t k = 0; k < m2.ncol(); ++k){
            double v = 0;
            for (size_t j = 0; j < m1.ncol(); ++j){
                 v += m1(i, j) * m2(j, k);
            }
            ret(i, k) = v;
        }
    }
    return ret;
 }

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, size_t tsize){
    is_validate_matrix(m1, m2);

    Matrix ret(m1.nrow(), m2.ncol());

    for (size_t ti = 0; ti < m1.nrow(); ti += tsize){
        size_t imax = std::min(ti + tsize, m1.nrow());
        for (size_t tj = 0; tj < m2.ncol(); tj += tsize){
            size_t jmax = std::min(tj + tsize, m2.ncol());
            for (size_t tk = 0; tk < m1.ncol(); tk += tsize){
                 size_t kmax = std::min(tk + tsize, m1.ncol());
                for (size_t k = tk; k < kmax; ++k){
                    for (size_t i = ti; i < imax; ++i){
                        for (size_t j = tj; j < jmax; ++j){
                             ret(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

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
         n);

     return ret;
 }

PYBIND11_MODULE(_matrix, m)
 {
     m.doc() = "matrix implementation";
     m.def("multiply_naive", &multiply_naive);
     m.def("multiply_tile", &multiply_tile);
     m.def("multiply_mkl", &multiply_mkl);
     m.def("bytes", &bytes);
     m.def("allocated", &allocated);
     m.def("deallocated", &deallocated);

     pybind11::class_<Matrix>(m, "Matrix")
         .def(pybind11::init<size_t, size_t>()) // call the constructor
         .def(pybind11::self == pybind11::self)       // operator overloading (==)
         .def_property_readonly("nrow", &Matrix::nrow)
         .def_property_readonly("ncol", &Matrix::ncol)
         .def("__getitem__", &Matrix::getitem)
         .def("__setitem__", &Matrix::setitem)
         .def("__repr__", &Matrix::str);
 } 

/*
PYBIND11_MODULE(_matrix, m){
     m.doc() = "matrix C++ implementation";
     
     m.def("multiply_naive", &multiply_naive);
     m.def("multiply_tile", &multiply_tile);
     m.def("multiply_mkl", &multiply_mkl);

     m.def("bytes", &bytes);
     m.def("allocated", &allocated);
     m.def("deallocated", &deallocated);

     pybind11::class_<Matrix>(m, "Matrix")
	     .def(pybind11::init<size_t, size_t>())
             .def("__getitem__", &Matrix::getitem)
	     .def("__setitem__", &Matrix::setitem)
             .def("__repr__", &Matrix::str)
     	     .def_property_readonly("nrow", &Matrix::nrow)
    	     .def_property_readonly("ncol", &Matrix::ncol);
 }*/ 
/*
int main(){
    //std::vector<double, MyAllocator<double>> m_vec = vector<double, MyAllocator<double>>(alloc);
    std::cout<<alloc<<std::endl;

    Matrix m2(2, 5);
    
    //m2.m_vec(alloc);
    std::vector<double, MyAllocator<double>>res(alloc);
    for (size_t it=0; it<50; ++it)
    {
        res.push_back(it);
    }
    //m2.setitem(std::pair<double, double>(1,2), 0.5);
    std::cout<<alloc<<std::endl;


    std::cout<<"ji"<<std::endl;
    Matrix m1(2, 5);
    //Matrix m2(2, 5);
    m1.setitem(std::pair<int, int>(1,2), 0.5);
    std::cout<<alloc<<std::endl;

    //if(m1 == m2) std::cout<<"true";
    std::string tmp;
    tmp = m1.str();
    std::cout<<tmp;
    std::cout<<m1.ncol()<<std::endl;
    double* address1 = m1.addr();
    //double* address2 = m2.addr();

    std::cout<<address1<<std::endl;
    //std::cout<<address2<<std::endl;

    return 0;
}*/

