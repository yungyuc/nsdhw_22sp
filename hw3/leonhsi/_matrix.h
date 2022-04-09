#include <vector>

using namespace std;

class Matrix{
public:
	//constructor
	Matrix() : m_nrow(0), m_ncol(0) {};

	Matrix(size_t nrow, size_t ncol) 
		: m_nrow(nrow), m_ncol(ncol){
		reset_buffer(nrow, ncol);
	}

	//copy constructor
	Matrix(Matrix const & other) 
		: m_nrow(other.m_nrow), m_ncol(other.m_ncol){
		reset_buffer(other.m_nrow, other.m_ncol);
		for(size_t i=0; i<m_nrow; i++){
			for(size_t j=0; j<m_ncol; j++){
				(*this)(i, j) = other(i, j);
			}
		}
	}

	//destructor
	~Matrix(){
		reset_buffer(0, 0);
	}

	//operator overloading
	Matrix & operator=(vector<double> const &vec){
		if(size() != vec.size()){
			throw out_of_range("number of elements mismatch");
		}

		size_t k = 0;
		for(size_t i=0; i<m_nrow; i++){
			for(size_t j=0; j<m_ncol; j++){
				(*this)(i,j) = vec[k];
				k++;
			}
		}

		return *this;
	}

	Matrix & operator=(Matrix const & other){
		if(this == &other) {return *this;}
		reset_buffer(other.m_nrow, other.m_ncol);
		for(size_t i=0; i<m_nrow; i++){
			for(size_t j=0; j<m_ncol; j++){
				(*this)(i, j) = other(i, j);
			}
		}

		return *this;
	}

	double operator() (size_t row, size_t col) const {return m_buffer[index(row, col)];}
	double & operator() (size_t row, size_t col) {return m_buffer[index(row, col)];}
	
	double operator[] (size_t idx) const {return m_buffer[idx];}
	double & operator[] (size_t idx) {return m_buffer[idx];}

	//member function
	size_t size() const {return m_nrow * m_ncol;}
	size_t index(size_t row, size_t col) const {return row * m_ncol + col;}
	size_t nrow() const {return m_nrow;}
	size_t ncol() const {return m_ncol;}
	double getvalue(pair<size_t, size_t> index) {return (*this)(index.first, index.second);}
	void setvalue(pair<size_t, size_t> index, const double value) {(*this)(index.first, index.second) = value;}
	double *val_addr() {return &m_buffer[0];}


//private:
	size_t m_nrow = 0;
	size_t m_ncol = 0;
	double *m_buffer = nullptr;
	
	void reset_buffer(size_t nrow, size_t ncol){
		if(m_buffer){
			delete[] m_buffer;
		}
		
		size_t nelement = nrow * ncol;
		m_buffer = nelement ? new double[nelement] : nullptr;
		m_nrow = nrow;
		m_ncol = ncol;

		for(size_t i=0; i<nelement; i++){
			m_buffer[i] = 0;
		}
	}
};

template<size_t N>
struct Block
{
    static constexpr const size_t NDIM = N;

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block<N> & operator= (double v)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block<N> & operator+= (Block<N> const & other)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    double m_buffer[N * N];
};

template<size_t N>
struct Tiler
{
    static constexpr const size_t NDIM = N;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block<N> m_mat1; // row-major
    Block<N> m_mat2; // column-major
    Block<N> m_ret; // row-major
};
