#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <bits/stdc++.h>
#include <cblas.h>



//Modified from the slide: https://yyc.solvcon.net/en/latest/nsd/07mem/mem.html#byte-counter
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

		std::size_t bytes() const 
		{ 
			return m_impl->allocated - m_impl->deallocated; 
		}

		std::size_t allocated() const 
		{ 
			return m_impl->allocated; 
		}

		std::size_t deallocated() const
		{
			return m_impl->deallocated; 
		}

		/* This is for debugging. */
		std::size_t refcount() const 
		{
			return m_impl->refcount; 
		}

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
};

template <class T>
struct CustomAllocator
{

	using value_type = T;

	// Just use the default constructor of ByteCounter for the data member
	// "counter".
	CustomAllocator() = default;

	template <class U> constexpr
		CustomAllocator(const CustomAllocator<U> & other) noexcept
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

}; /* end struct CustomAllocator */

////////////////////////////////////////////////////////////


static CustomAllocator<double> allocator;
size_t bytes() {
	return allocator.counter.bytes();
}

size_t allocated() {
	return allocator.counter.allocated(); 
}

size_t deallocated() {
	return allocator.counter.deallocated();
}

class Matrix{
	private:
		std::vector<double, CustomAllocator<double>> m_array;	
		size_t m_row_num, m_col_num;

	public:
		Matrix(size_t row_num, size_t col_num) : m_array(allocator){
			m_row_num = row_num;
			m_col_num = col_num;
			m_array.resize(row_num * col_num);
		}

		size_t nrow() const{return m_row_num;}
		size_t ncol() const{return m_col_num;}

		double* data(){ 
			return m_array.data(); 
		}

		double operator()(size_t row, size_t col)const{
			return m_array[row * m_col_num + col];
		}
		double &operator()(size_t row, size_t col){
			return m_array[row * m_col_num + col];
		}

		double getitem(std::pair<size_t, size_t> index){
			return (*this)(index.first, index.second);
		}

		void setitem(std::pair<size_t, size_t> index, double value) {
			(*this)(index.first, index.second) = value;
		}

};

bool operator== (Matrix const &mat1, Matrix const &mat2){
	if(mat1.ncol() != mat2.ncol() || mat1.nrow() != mat2.ncol()) return false;

	for(size_t i=0; i<mat1.nrow(); i++){
		for(size_t j=0; j<mat1.ncol(); j++){
			if(mat1(i, j) != mat2(i, j)) return false;
		}
	}
	return true;
}


Matrix multiply_naive(Matrix& mat1, Matrix& mat2){
	Matrix res(mat1.nrow(), mat2.ncol());
	for(size_t k=0; k < mat2.ncol(); k++)
		for(size_t i=0; i < mat1.nrow(); i++)
			for(size_t j=0; j < mat1.ncol(); j++)
				res(i, k) += mat1(i,j) * mat2(j,k);

	return res;
}

Matrix multiply_tile(Matrix& mat1, Matrix& mat2, size_t tsize){
	Matrix res(mat1.nrow(), mat2.ncol());

	//tile
	for(size_t ti = 0; ti < mat1.nrow(); ti += tsize)
		for(size_t tj = 0; tj < mat1.ncol(); tj += tsize)
			for(size_t tk = 0; tk < mat2.ncol(); tk += tsize)
				//each tile consider as a small matrix
				for(size_t i = ti; i < std::min(ti + tsize, mat1.nrow()); i++)
					for(size_t k = tk; k < std::min(tk + tsize, mat2.ncol()); k++){
						double tmp = 0.0;
						for(size_t j = tj; j < std::min(tj + tsize, mat1.ncol()); j++)
							tmp += mat1(i, j) * mat2(j, k);
						res(i, k) += tmp;
					}
	return res;
}

Matrix multiply_mkl(Matrix& mat1, Matrix& mat2){
	Matrix res(mat1.nrow(), mat2.ncol());
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
			mat1.nrow(), mat2.ncol(), mat1.ncol(), 
			1.0, mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 0.0, res.data(), mat2.ncol());		
	return res;
}

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 plugin";
	pybind11::class_<Matrix>(m, "Matrix")
		.def(pybind11::init<size_t, size_t>())
		.def(pybind11::self == pybind11::self)	
		.def("__getitem__", &Matrix::getitem)
		.def("__setitem__", &Matrix::setitem)
		.def_property_readonly("nrow", &Matrix::nrow)
		.def_property_readonly("ncol", &Matrix::ncol);

	m.def("multiply_naive", &multiply_naive);
	m.def("multiply_tile", &multiply_tile);
	m.def("multiply_mkl", &multiply_mkl);
	m.def("bytes", &bytes);
	m.def("allocated", &allocated);
	m.def("deallocated", &deallocated);
}
