//
//  matrix.cpp
//  nsd-hw3
//
//  Created by MinChunCho on 2022/3/25.
//

#include "matrix.hpp"

Allocator<double> alloc;

Matrix::Matrix(size_t const& row, size_t const& col)
    : row_(row), col_(col), vec_(alloc)
{
    if(!this->vec_.empty()){
        this->vec_.clear();
    }
    
    this->vec_.resize(row*col);
}

Matrix::Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec)
    :row_(row), col_(col), vec_(alloc)
{
    if(vec.size() != row*col) throw std::out_of_range("size mismatches");
    
    this->vec_.clear();
    this->vec_.shrink_to_fit();

    for(auto const& item : vec){
        this->vec_.push_back(item);
    }
}

// copy constructor
Matrix::Matrix(Matrix const& mat)
{
    this->row_ = mat.row_;
    this->col_ = mat.col_;
    this->vec_ = mat.vec_;
}

// copy assignment
Matrix& Matrix::operator=(Matrix const& mat)
{
    if(this == &mat) return *this;
    
    this->row_ = mat.row_;
    this->col_ = mat.col_;
    this->vec_ = mat.vec_;
    
    return *this;
}

// move constructor
Matrix::Matrix(Matrix && mat)
    : vec_(std::move(mat.vec_))
{
    std::swap(this->row_, mat.row_);
    std::swap(this->col_, mat.col_);
    // std::move(mat.vec_.begin(), mat.vec_.end(), std::back_inserter(this->vec_));
}

// move assignment
Matrix& Matrix::operator=(Matrix && mat)
{
    if(this == &mat) return *this;
    
    std::swap(this->row_, mat.row_);
    std::swap(this->col_, mat.col_);
    std::move(mat.vec_.begin(), mat.vec_.end(), std::back_inserter(this->vec_));
    
    return *this;
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

double& Matrix::operator()(size_t const& i, size_t const& j)
{
    return this->vec_[i*this->col_ + j];
}

double Matrix::operator()(size_t const& i, size_t const& j) const
{
    return this->vec_[i*this->col_ + j];
}

bool Matrix::operator==(Matrix const& mat) const
{
    if(this->col_ != mat.col_ || this->row_ != mat.row_) return false;
    
    for(size_t i=0; i<this->size(); i++){
        // a floating-point precision issue
        // if(this->vec_[i] != mat.vec_[i]) return false;
        if(fabs(this->vec_[i]-mat.vec_[i]) > 0.000001f) return false;
    }
    
    return true;
}

bool Matrix::bound_check(size_t const& i, size_t const& j)
{
    return i<this->row_ && i>=0 && j<this->col_ && j>=0;
}

double Matrix::get_element(std::pair<size_t, size_t> index)
{
    return (*this)(index.first, index.second);
}

void Matrix::set_element(std::pair<size_t, size_t> index, const double& item)
{
    (*this)(index.first, index.second) = item;
}

std::string Matrix::get_matrix_str()
{
    std::string s;
    for(size_t i=0; i<row_; i++){
        for(size_t j=0; j<col_; j++){
            s += std::to_string((*this)(i, j)) + ' ';
        }
        s += '\n';
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, Matrix mat)
{
    for(size_t i=0; i<mat.row_; i+=mat.col_){
        for(size_t j=0; j<mat.col_; j++){
            std::cout << mat(i, j) << " ";
        }
        std::cout << std::endl;
    }
    
    return os;
}

Matrix multiply_naive(Matrix const& mat1, Matrix const& mat2)
{
    if(mat1.col() != mat2.row()){
        throw std::out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(mat1.row(), mat2.col());

    for(size_t i=0; i<mat1.row(); i++){
        for(size_t j=0; j<mat2.col(); j++){
            double v = 0;
            for(size_t k=0; k<mat1.col(); k++){
                v += mat1(i, k)*mat2(k, j);
            }
            ret(i,j) = v;
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize)
{
    size_t m1_col = mat1.col(), m1_row = mat1.row(), m2_row = mat2.row(), m2_col = mat2.col();

    if(m1_col != m2_row){
        throw std::out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(m1_row, m2_col);
    size_t i_size, j_size, k_size;
    double v=0;

    for(size_t ti=0; ti<m1_row; ti+=tsize){
        i_size = std::min(m1_row, ti+tsize);
        for(size_t tj=0; tj<m2_col; tj+=tsize){
            j_size = std::min(m2_col, tj+tsize);
            for(size_t tk=0; tk<m1_col; tk+=tsize){
                k_size = std::min(m1_col, tk+tsize);
                for(size_t i=ti; i<i_size; ++i){
                    size_t i_col = i * mat1.col_;
                    for(size_t j=tj; j<j_size; ++j){
                        v = 0;
                        for(size_t k=tk; k<k_size; ++k){
                            v += mat1.vec_[i_col+k] * mat2.vec_[k*mat2.col_+j];
                        }
                        ret.vec_[i_col+j] += v;
                    }
                }
            }
        }
    }
    
    return ret;
}

Matrix multiply_mkl(Matrix& mat1, Matrix& mat2)
{
    if(mat1.col() != mat2.row()){
        throw std::out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(mat1.row(), mat2.col());
    
    const size_t m = mat1.row(), n = mat2.col(), k = mat1.col();
    double alpha = 1.0, beta = 0.0;
    double* A = mat1.addr();
    double* B = mat2.addr();
    double* C = ret.addr();

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);

    return ret;
}

std::size_t bytes()
{
    return alloc.counter_.bytes();
}

std::size_t allocated()
{
    return alloc.counter_.allocated();
}

std::size_t deallocated()
{
    return alloc.counter_.deallocated();
}