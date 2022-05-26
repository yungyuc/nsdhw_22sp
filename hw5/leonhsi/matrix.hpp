#include <bits/stdc++.h>
#include <cblas.h>

class Matrix{
private:
public:
	std::vector<double> m_array;	
	size_t m_row_num, m_col_num;

public:
	Matrix(size_t row_num, size_t col_num){
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
    for(size_t k=0; k < mat2.ncol(); k++){
	    for(size_t i=0; i < mat1.nrow(); i++){
	        for(size_t j=0; j < mat1.ncol(); j++){
		        res(i, k) += mat1(i,j) * mat2(j,k);
            }
        }
    }

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