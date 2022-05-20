#include <iostream>
#include "matrix.hpp"

int main()
{
    const size_t n = 2;
    int status;

    std::cout << ">>> Solve Ax=b (row major)" << std::endl;
    Matrix mat(n, n);
    mat(0,0) = 3; mat(0,1) = 5;
    mat(1,0) = 2; mat(1,1) = 1;    
    Matrix b(n, 1);
    b(0,0) = 1; b(1,0) = 1;
    Matrix c(n, 1);
    
    std::vector<int> ipiv(n);

    std::cout << "A:" << mat << std::endl;
    std::cout << "b:" << b << std::endl;

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

    std::cout << "solution x:" << b << std::endl;
}