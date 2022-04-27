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

class ByteCounter {
 public:
 	ByteCounter() : m_allocated(0), m_deallocated(0) {};
 	ByteCounter(ByteCounter const& other) : m_allocated(other.m_allocated), m_deallocated(other.m_deallocated) {};
 	ByteCounter(ByteCounter && other)
     {
         std::swap(m_allocated, other.m_allocated);
         std::swap(m_deallocated, other.m_deallocated);
     }
 	ByteCounter& operator=(ByteCounter const& other)
     {
         if (&other != this)
         {
             m_allocated = other.m_allocated;
             m_deallocated = other.m_deallocated;
         }
         return *this;
     }
 	ByteCounter& operator=(ByteCounter && other)
     {
         if (&other != this)
         {
             std::swap(m_allocated, other.m_allocated);
             std::swap(m_deallocated, other.m_deallocated);
         }
         return *this;
     }
 	~ByteCounter() {};

 	void increase(size_t bytes) { m_allocated += bytes; }
 	void decrease(size_t bytes) { m_deallocated += bytes; }

 	size_t bytes() const { return m_allocated - m_deallocated; };
 	size_t allocated() const { return m_allocated; };
 	size_t deallocated() const { return m_deallocated; };

 private:
     size_t m_allocated;
     size_t m_deallocated;
 };


template<class T>
 class Allocator {
 public:
 	typedef T value_type;

     Allocator() = default;

 	T* allocate(size_t n)
     {
         if (n > std::numeric_limits<size_t>::max()/sizeof(T))
         {
             throw std::bad_alloc();
         }

         const size_t bytes = n*sizeof(T);

         T* p = static_cast<T*>(std::malloc(bytes));
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

     void deallocate(T* p, size_t n) noexcept
     {
         std::free(p);

         const size_t bytes = n * sizeof(T);
         counter.decrease(bytes);
     }

     static ByteCounter counter;
 };

 template <class T>
 ByteCounter Allocator<T>::counter{};

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


Allocator<double> alloc;

 class Matrix
 {
 public:
     Matrix(size_t row, size_t col) : m_rows(row), m_cols(col), m_vec(alloc)
     {
         m_vec.clear();
         m_vec.resize(row * col, 0);
     }

     // copy constructor
     Matrix(Matrix const &other)
     {
         this->m_rows = other.m_rows;
         this->m_cols = other.m_cols;
         this->m_vec = other.m_vec;
     }

     // move constructor
     Matrix(Matrix &&other) : m_vec(std::move(other.m_vec))
     {
         std::swap(this->m_rows, other.m_rows);
         std::swap(this->m_cols, other.m_cols);
     }

     // copy assignment operator
     Matrix &operator=(Matrix const &other)
     {
         if (this != &other)
         {
             this->m_rows = other.m_rows;
             this->m_cols = other.m_cols;
             this->m_vec = other.m_vec;
         }
         return *this;
     }

     // move assignment operator
     Matrix &operator=(Matrix &&other)
     {
         if (this != &other)
         {
             this->m_rows = other.m_rows;
             this->m_cols = other.m_cols;
             std::move(other.m_vec.begin(), other.m_vec.end(), std::back_inserter(this->m_vec));
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
     double operator()(size_t row, size_t col) const
     {
         return m_vec[m_cols * row + col];
     }
     double &operator()(size_t row, size_t col)
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
     std::string str() const
     {
	 std::string s = "";
         for (size_t i = 0; i < m_rows; ++i)
         {
             for (size_t j = 0; j < m_cols; ++j)
                 s += std::to_string(m_vec[i * m_cols + j]) + ' ';
             s += '\n';
         }
         return s;
     }

 private:
     std::vector<double, Allocator<double>> m_vec;
     size_t m_rows;
     size_t m_cols;
 };

 size_t bytes()
 {
     return alloc.counter.bytes();
 }

 size_t allocated()
 {
     return alloc.counter.allocated();
 }

 size_t deallocated()
 {
     return alloc.counter.deallocated();
 }


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

     // Ref: https://pybind11.readthedocs.io/en/stable/advanced/classes.html
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

