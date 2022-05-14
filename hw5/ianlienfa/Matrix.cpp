#include <iostream>
#include "Matrix.hpp"
#include "Util.h"

long long Tracker::tile_res_time = 0; 
long long Tracker::tile_construct_time = 0; 
long long Tracker::tile_mul_time = 0; 
long long Tracker::tile_fill_time = 0;
long long Tracker::tile_load_time = 0; 
std::chrono::steady_clock::time_point Tracker::begin =  std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point Tracker::end =  std::chrono::steady_clock::now();

Matrix Matrix::tiled_mul(const Matrix &mat, int tile_size){

    if(this->mCols != mat.mRows){
        throw "Matrixs are not compatible";
    }
    Matrix res(this->mRows, mat.mCols);

    // Adjust tile_size if not appropriate
    if((this->mCols % tile_size) || (this->mRows % tile_size) || (mat.mCols % tile_size))
    {
        int tmp_tile_size = std::__gcd(this->mCols, this->mRows);
        tmp_tile_size = std::__gcd(tmp_tile_size, mat.mCols);
        
        // find nearest tile_size
        while(tmp_tile_size % tile_size){
            tile_size--;
        }
    }

    int trow1 = this->mRows / tile_size;
    int tcol1 = this->mCols / tile_size;
    int tcol2 = mat.mCols / tile_size;


    Matrix mat2_tmp(tile_size, tile_size);

    for(int i = 0; i < trow1; i++){
        for(int k = 0; k < tcol2; k++){              
            // Do tile multiplication       
            for(int j = 0; j < tcol1; j++){
                direct_mul(*this, i, j, mat, j, k, res, tile_size, mat2_tmp);                
            }
        }
    }
    
    return res;
}

// void Matrix::direct_mul(const Matrix& mat1, int row1, int col1, const Matrix& mat2, int row2, int col2, Matrix &tile_res)
// {   
//     int tile_size = tile_res.mRows;
//     if(col1 != row2){
//         throw "Matrixs are not compatible";
//     }
    
//     int base_i1 = row1 * tile_size;
//     int base_j1 = col1 * tile_size;
//     int base_i2 = row2 * tile_size;
//     int base_j2 = col2 * tile_size;
//     for(int i = 0; i < tile_size; i++){
//         for(int j = 0; j < tile_size; j++){
//             for(int k = 0; k < tile_size; k++){
//                 tile_res(i, j) += mat1(base_i1+i,base_j1+k) * mat2(base_i2+k,base_j2+j);
//             }
//         }
//     }    
// }

void Matrix::direct_mul(const Matrix& mat1, int row1, int col1, const Matrix& mat2, int row2, int col2, Matrix &tile_res, int tile_size, Matrix &mat2_tmp)
{   
    if(col1 != row2){
        throw "Matrixs are not compatible";
    }
    
    int base_i1 = row1 * tile_size;
    int base_j1 = col1 * tile_size;
    int base_j2 = col2 * tile_size;    
    

    // Load mat2 tmp
    for(int i = 0; i < tile_size; i++)
    {
        int Base_i = i * tile_size;
        for(int j = 0; j < tile_size; j++)
        {
            mat2_tmp.buffer[Base_i + j] = mat2(base_j1 + j, base_j2 + i);
        }
    }

    for(int i = 0; i < tile_size; i++){
        for(int j = 0; j < tile_size; j++){
            int Base_j = j * tile_size;
            double accu = 0;
            for(int k = 0; k < tile_size; k++){
                accu += mat1(base_i1+i,base_j1+k) * mat2_tmp.buffer[Base_j + k];
            }
            tile_res(base_i1 + i, base_j2 + j) += accu;
        }
    }    
}



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
        .def(py::init<int, int>())
        .def("show", &Matrix::show, py::arg("name") = "")
        .def("brief", &Matrix::brief, py::arg("name") = "")
        .def(py::self == py::self)
        .def_readonly("nrow", &Matrix::mRows)
        .def_readonly("ncol", &Matrix::mCols)
        .def("__getitem__",
             [](const Matrix &s, std::tuple<int, int> t) {
                 int i, j;
                 std::tie(i, j) = t;
                 if(i < s.mRows && j < s.mCols)
                     return s(i, j);
                 else
                     throw "Index out of range";
             })
        .def("__setitem__",
             [](Matrix &s, std::tuple<int, int> t, float val) {
                 int i, j;
                 std::tie(i, j) = t;
                 if(i < s.mRows && j < s.mCols)
                    s(i, j) = val;
                 else
                     throw "Index out of range";
             });
    m.def("multiply_naive", &Matrix::multiply_naive);
    m.def("multiply_tile", &Matrix::multiply_tile);
    m.def("multiply_mkl", &Matrix::multiply_mkl);
    m.def("bytes", &MyAllocator<double>::bytes);
    m.def("deallocated", &MyAllocator<double>::deallocated);
    m.def("allocated", &MyAllocator<double>::allocated);

}

#endif 