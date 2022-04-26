#include "_matrix.hpp"

MyAllocator<double> alloc;

Matrix::Matrix(size_t const& row, size_t const& col): number_row(row), number_col(col), vec_(alloc)
{
    if(!this->vec_.empty()){
        this->vec_.clear();
    }

    this->vec_.resize(row * col);
}

Matrix::Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec)
    :number_row(row), number_col(col), vec_(alloc)
{
    if(vec.size() != row*col) throw std::out_of_range("size mismatches");

    this->vec_.clear();
    this->vec_.shrink_to_fit();

    for(auto const& item : vec){
        this->vec_.push_back(item);
    }
}

Matrix::Matrix(const Matrix &matrix)
{
    this->number_row = matrix.number_row;
    this->number_col = matrix.number_col;
    this->vec_ = matrix.vec_;
}

bool Matrix::operator== (Matrix const &matrix) const 
{
    for (size_t i = 0; i < number_row; i++) 
        for (size_t j = 0; j < number_col; j++)
            if (vec_[i*number_col + j] != matrix(i, j)) return false;

    return true;
}

Matrix& Matrix::operator= (const Matrix& matrix) 
{
    if(this == &matrix) return *this;

    this->number_row = matrix.number_row;
    this->number_col = matrix.number_col;
    this->vec_ = matrix.vec_;

    return *this;
}

// move constructor
Matrix::Matrix(Matrix && mat)
    : vec_(std::move(mat.vec_))
{
    std::swap(this->number_row, mat.number_row);
    std::swap(this->number_col, mat.number_col);
}

Matrix& Matrix::operator=(std::vector<double> const& vec)
{
    this->vec_.clear();
    this->vec_.shrink_to_fit();

    for(auto const& item : vec){
        this->vec_.push_back(item);
    }

    return *this;
}

double  Matrix::operator() (size_t const& row, size_t const& col) const 
{ 
    return this->vec_[row*this->number_col + col]; 
}

double & Matrix::operator() (size_t const& row, size_t const& col)
{ 
    return this->vec_[row*this->number_col + col]; 
}

double * Matrix::get_matrix_buffer()
{ 
    return &vec_[0]; 
}

double Matrix::get_element(std::pair<size_t, size_t> index)
{
    return (*this)(index.first, index.second);
}

void Matrix::set_element(std::pair<size_t, size_t> index, const double& item)
{
    (*this)(index.first, index.second) = item;
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

Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2)
{
    validate_multiplication(matrix1, matrix2);

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    for (size_t row = 0; row < ret.nrow(); row++) 
    {
        for (size_t column = 0; column < ret.ncol(); column++) 
        {
            double value = 0;
            for (size_t k = 0; k < matrix1.ncol(); k++) {
                value += matrix1(row, k) * matrix2(k, column);
            }
            ret(row, column) = value;
        }
    }

    return ret;

}

Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize)
{
    validate_multiplication(matrix1, matrix2);

    size_t row_max = matrix1.nrow();
    size_t col_max = matrix2.ncol();
    size_t inner_max = matrix1.ncol();
    Matrix ret(row_max, col_max);

    for (size_t row = 0; row < row_max; row += tilesize) 
    {
        size_t imax = std::min(row_max, row + tilesize);
        
        for (size_t col = 0; col < col_max; col += tilesize) 
        {
            size_t jmax = std::min(col_max, col + tilesize);
            
            for (size_t inner = 0; inner < inner_max; inner += tilesize) 
            {
                size_t kmax = std::min(inner_max, inner + tilesize);

                for (size_t i = row; i < imax; ++i) 
                {
                    size_t base1 = i * inner_max;
                    for (size_t j = col; j < jmax; ++j)
                    {
                        size_t index = i * col_max + j;
                        for (size_t k = inner; k < kmax; ++k) 
                        {   
                            ret.vec_[index] += matrix1.vec_[base1 + k] * matrix2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;

}

Matrix multiply_mkl(Matrix & matrix1, Matrix & matrix2)
{
    validate_multiplication(matrix1, matrix2);

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        matrix1.nrow(),
        matrix2.ncol(),
        matrix1.ncol(),
        1.0,
        matrix1.get_matrix_buffer(),
        matrix1.ncol(),
        matrix2.get_matrix_buffer(),
        matrix2.ncol(),
        0.0,
        ret.get_matrix_buffer(),
        ret.ncol()
    );

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

std::size_t bytes()
{
    return alloc.counter.bytes();
}

std::size_t allocated()
{
    return alloc.counter.allocated();
}

std::size_t deallocated()
{
    return alloc.counter.deallocated();
} 