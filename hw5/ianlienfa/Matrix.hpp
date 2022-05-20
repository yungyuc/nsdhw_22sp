#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "Tiler.h"
#include "Allocator.h"
using std::cout, std::endl;
using std::ostream;
using std::string, std::vector;
using std::copy;
namespace py = pybind11;

int constexpr TILE_SIZE = 100; 

#define mRows nrow
#define mCols ncol

struct Tiler;
struct Matrix {
        
    vector<double, MyAllocator<double>> buffer;
    bool trans;
    bool col_maj;    
    size_t nrow, ncol;    

    Matrix(){
        mRows = 0;
        mCols = 0;        
    }

    ~Matrix(){
        buffer.clear();
    }

    Matrix(size_t row, size_t col): mRows(row), mCols(col) {
        buffer.assign(row*col, 0);
        for(size_t i = 0; i < mRows * mCols; i++)
            buffer[i] = 0;
        trans = false;
    }
    
    Matrix(const Matrix & mat): Matrix(mat.mRows, mat.mCols)
    {       
        buffer.assign(mat.mRows * mat.mCols, 0);
        if(!mat.trans)
        {            
            copy(mat.buffer.begin(), mat.buffer.begin()+(mat.mRows * mat.mCols), buffer.begin());
        }
        else
        {
            for(size_t i = 0; i < mat.mRows; i++)
            {
                for(size_t j = 0; j < mat.mCols; j++)
                {
                    buffer[i * mat.mCols + j] = mat(i, j);                
                }
            }
        }
    }

    Matrix(size_t row, size_t col, std::initializer_list<double> l): mRows(row), mCols(col) {
        vector<double> v(l);
        buffer.assign(mRows * mCols, 0);
        std::copy(v.begin(), v.end(), buffer.begin());
        trans = false;
    }

    Matrix(size_t row, size_t col, vector<double> v): mRows(row), mCols(col) {        
        buffer.assign(mRows * mCols, 0);
        std::copy(v.begin(), v.end(), buffer.begin());
        trans = false;
    }

    Matrix& transpose(){
        std::swap(mRows, mCols);
        trans = (trans) ? false: true;
        return *this;
    }

    double operator()(size_t row, size_t col) const {
        if(trans)
            return buffer[row + mRows * col];
        return buffer[row * mCols + col];
    }

    double& operator()(size_t row, size_t col) {
        if(trans)
            return buffer[row + mRows * col];
        return buffer[row * mCols + col];
    }

    void clean()
    {
        for(size_t i = 0; i < mRows * mCols; i++)
            buffer[i] = 0;   
        trans = false;         
    }

    bool isTrans() const {return trans;}

    const Matrix& operator=(const Matrix & mat)
    {        
        if(this == &mat)
            return *this;        
        buffer.assign(mRows * mCols, 0);
        copy(mat.buffer.begin(), mat.buffer.begin()+(mat.mRows * mat.mCols), buffer.begin());
        mRows = mat.mRows;
        mCols = mat.mCols;
        return *this;
    }
    
    void show(string name=""){
        cout << "Matrix<" << mRows << ", " << mCols << ">" << " " << name << endl;
        for (size_t i = 0; i < mRows; i++) {
            for (size_t j = 0; j < mCols; j++) {
                cout << this->operator()(i, j) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void brief(string name=""){
        cout << "Matrix<" << mRows << ", " << mCols << ">" << " " << name << endl;
        for (size_t i = 0; i < std::min(mRows, size_t(5)); i++) {
            for (size_t j = 0; j < std::min(mCols, size_t(10)); j++) {
                cout << this->operator()(i, j) << " ";
            }
            if(mCols > 10)
                cout << "...";
            cout << endl;            
        }
        if(mRows > 5)
            cout << "..." << endl;
        cout << endl;
    }

    friend ostream& operator<<(ostream& os, const Matrix& dt);

    vector<int> operator*(const vector<int> &v){
        vector<int> res;        
        for(size_t i = 0; i < mRows; i++){
            int accu = 0;
            for(size_t j = 0; j < mCols; j++){
                accu += (this->operator()(i, j) * v[j]);
            }
            res.push_back(accu);
        }
        return res;
    }

    void operator+=(const Matrix &mat){
        for(size_t i = 0; i < mRows; i++){
            for(size_t j = 0; j < mCols; j++){
                this->operator()(i, j) += mat(i, j);
            }
        }
    }

    Matrix operator*(const Matrix &mat){
        if(this->mCols != mat.mRows){
            throw "Matrixs are not compatible";
        }        
        Matrix res(this->mRows, mat.mCols);
        for(size_t i = 0; i < res.mRows; i++){            
            for(size_t j = 0; j < res.mCols; j++){
                double accu = 0;      
                for(size_t k = 0; k < mat.mRows; k++){         
                    accu += this->operator()(i, k) * mat(k, j);
                }
                res(i, j) = accu;
            }
        }     
        return res;
    }

    void partial_fill(int tile_size, const Matrix &mat, int row, int col)
    {
        // fill the tile to the corresponding position
        for(int i = 0; i < tile_size; i++){
            for(int j = 0; j < tile_size; j++){
                this->operator()(row * tile_size + i, col * tile_size + j) = mat(i, j);
            }
        }
    }
    
    bool operator==(const Matrix &mat) const{
        if(this->mRows != mat.mRows || this->mCols != mat.mCols)
            return false;
        for(size_t i = 0; i < mRows; i++){
            for(size_t j = 0; j < mCols; j++){
                if(this->operator()(i, j) != mat(i, j))
                    return false;
            }
        }
        return true;
    }
 
    Matrix tiled_mul(const Matrix &mat, int tile_size);
    double* data(){return buffer.data();}    

    static Matrix* _multiply_naive(Matrix &mat1, Matrix &mat2)
    {         
        if(mat1.mCols != mat2.mRows){
            throw "Matrixs are not compatible";
        }        
        Matrix* mult = new Matrix(mat1.mRows, mat2.mCols);
        Matrix& res = *mult;
        for(size_t i = 0; i < res.mRows; i++){            
            for(size_t j = 0; j < res.mCols; j++){
                double accu = 0;      
                for(size_t k = 0; k < mat2.mRows; k++){         
                    accu += mat1(i, k) * mat2(k, j);
                }
                res(i, j) = accu;
            }
        }     
        return mult;       
    }

    static Matrix _multiply_tile(Matrix &mat1, Matrix &mat2, int tsize)
    {
        return mat1.tiled_mul(mat2, tsize);
    }

    void direct_mul(const Matrix& mat1, int row1, int col1, const Matrix& mat2, int row2, int col2, Matrix &tile_res, int tile_size, Matrix &mat2_tmp);

    static Matrix* _multiply_mkl(Matrix &mat1, Matrix &mat2)
    {
        Matrix &mat = mat1;
        Matrix &b = mat2;
        Matrix* mult = new Matrix(mat1.mRows, mat2.mCols);
        Matrix& c = *mult;
        cblas_dgemm(
        CblasRowMajor // int matrix_layout
        , CblasNoTrans // TRANSA
        , CblasNoTrans // TRANSB
        , mat.mRows // lapack_int M
        , b.mCols // lapack_int N
        , mat.mCols // lapack_int K
        , 1.0 // double alpha
        , mat.data() // double * A
        , mat.mRows // lapack_int lda
        , b.data() // double * B
        , b.mRows // lapack_int ldb
        , 0.0 // double beta
        , c.data() // double * C
        , c.mRows // lapack_int ldc
        );
        return mult;
    }

    static bool __eq__(Matrix const & self, Matrix const & other) { 
        return self == other; 
    }
    static double __getitem__(Matrix const & self, std::tuple<size_t, size_t> idx)
    { return self(std::get<0>(idx), std::get<1>(idx)); }
    static double __setitem__(Matrix & self, std::tuple<size_t, size_t> idx, double value)
    { return self(std::get<0>(idx), std::get<1>(idx)) = value; }
};  

Matrix* multiply_mkl(Matrix &mat1, Matrix &mat2);
Matrix* multiply_naive(Matrix &mat1, Matrix &mat2);
Matrix multiply_tile(Matrix &mat1, Matrix &mat2, int tsize);

#endif // define MATRIX_H

