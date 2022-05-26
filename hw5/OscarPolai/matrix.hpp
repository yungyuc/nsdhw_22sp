#include <cstdlib>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <cblas.h>


class Matrix{
 public:
    Matrix(size_t row, size_t col);
    const size_t nrow() const{ return m_rows;}
    const size_t ncol() const{ return m_cols;}
    double *addr();
    
     // rewrite operator
    double operator()(size_t row, size_t col) const{ return m_vec[m_cols * row + col];}
    double &operator()(size_t row, size_t col){ return m_vec[m_cols * row + col];}
    
    double getitem(std::pair<size_t, size_t> index);
    void setitem(std::pair<size_t, size_t> index, const double value);
    
    std::string str() const;

private:
    size_t m_rows;
    size_t m_cols;
    std::vector<double> m_vec;
};

Matrix::Matrix(size_t row, size_t col){
	m_rows = row;
	m_cols = col;
	m_vec.resize(row * col, 0);
}


double* Matrix::addr(){
       return m_vec.data();
}

bool operator== (Matrix const &mat1, Matrix const &mat2){
 	if(mat1.ncol() != mat2.ncol() || mat1.nrow() != mat2.ncol()) return false;

 	for(size_t i=0; i<mat1.nrow(); i++){
 		for(size_t j=0; j<mat1.ncol(); j++){
 			if(mat1(i, j) != mat2(i, j)) return false;
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
 				1.0, mat1.addr(), mat1.ncol(), mat2.addr(), mat2.ncol(), 0.0, res.addr(), mat2.ncol());		
 	return res;
 }

