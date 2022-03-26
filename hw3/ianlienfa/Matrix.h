#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using std::cout, std::endl;
using std::ostream;
using std::string, std::vector;
using std::copy;
namespace py = pybind11;

struct Matrix {
    double *buffer;
    bool trans;
    int mRows, mCols;
    
    Matrix(){
        mRows = 0;
        mCols = 0;
    }

    Matrix(int row, int col): mRows(row), mCols(col) {
        buffer = new double[mRows * mCols];
        trans = false;
    }
    
    Matrix(const Matrix & mat): Matrix(mat.mRows, mat.mCols)
    {       
        buffer = new double[mRows * mCols]; 
        if(!mat.trans)
        {            
            for(int i = 0; i < mat.mRows * mat.mCols; i++)
                cout << mat.buffer[i] << " ";
                cout << endl;
            copy(mat.buffer, mat.buffer+(mat.mRows * mat.mCols), buffer);
        }
        else
        {
            for(int i = 0; i < mat.mRows; i++)
            {
                for(int j = 0; j < mat.mCols; j++)
                {
                    buffer[i * mat.mCols + j] = mat(i, j);                
                }
            }
        }
    }

    Matrix(int row, int col, std::initializer_list<double> l): mRows(row), mCols(col) {
        vector<double> v(l);
        buffer = new double[mRows * mCols];
        std::copy(v.begin(), v.end(), buffer);
        trans = false;
    }

    Matrix(int row, int col, vector<double> v): mRows(row), mCols(col) {        
        buffer = new double[mRows * mCols];
        std::copy(v.begin(), v.end(), buffer);
        trans = false;
    }

    Matrix& transpose(){
        std::swap(mRows, mCols);
        trans = (trans) ? false: true;
        return *this;
    }

    double operator()(int row, int col) const {
        if(trans)
            return buffer[row + mRows * col];
        return buffer[row * mCols + col];
    }

    double& operator()(int row, int col) {
        if(trans)
            return buffer[row + mRows * col];
        return buffer[row * mCols + col];
    }

    bool isTrans() const {return trans;}

    const Matrix& operator=(const Matrix & mat)
    {        
        if(this == &mat)
            return *this;
        delete[] buffer;
        buffer = new double[mat.mRows * mat.mCols];
        copy(mat.buffer, mat.buffer+(mat.mRows * mat.mCols), buffer);
        mRows = mat.mRows;
        mCols = mat.mCols;
        return *this;
    }
    
    void show(string name=""){
        cout << "Matrix<" << mRows << ", " << mCols << ">" << " " << name << endl;
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < mCols; j++) {
                cout << this->operator()(i, j) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    friend ostream& operator<<(ostream& os, const Matrix& dt);

    vector<int> operator*(const vector<int> &v){
        vector<int> res;        
        for(int i = 0; i < mRows; i++){
            int accu = 0;
            for(int j = 0; j < mCols; j++){
                accu += (this->operator()(i, j) * v[j]);
            }
            res.push_back(accu);
        }
        return res;
    }

    Matrix operator*(const Matrix &mat){
        if(this->mCols != mat.mRows){
            throw "Matrixs are not compatible";
        }
        Matrix res(this->mRows, mat.mCols);
        for(int i = 0; i < res.mRows; i++){            
            for(int j = 0; j < res.mCols; j++){
                int accu = 0;      
                for(int k = 0; k < mat.mRows; k++){          
                    accu += this->operator()(i, k) * mat(k, j);
                }
                res(i, j) = accu;
            }
        }
        return res;
    }

    double* data(){return buffer;}    
};  

ostream& operator<<(ostream& os, const Matrix& dt)
{
    os << "Matrix<" << dt.mRows << ", " << dt.mCols << ">" << endl;
    for (int i = 0; i < dt.mRows; i++) {
        for (int j = 0; j < dt.mCols; j++) {
            os << dt.operator()(i, j) << " ";
        }
        os << endl;
    }
    os << endl;
    return os;
}

#ifdef FORPYBIND
PYBIND11_MODULE(_matrix, m) {
    m.attr("__name__") = "matrix";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<int, int, vector<double>>())
        .def("show", &Matrix::show, py::arg("name") = "");
}
#endif 

#endif // define MATRIX_H
