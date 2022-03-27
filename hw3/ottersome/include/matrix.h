#include <stdio.h>
#include <vector>

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);

    Matrix & operator=(std::vector<double> const & vec);
    Matrix(Matrix const & other);
    Matrix & operator=(Matrix const & other);
    Matrix(Matrix && other);

    Matrix & operator=(Matrix && other);
    ~Matrix();

    double   operator() (size_t row, size_t col) const;
    double & operator() (size_t row, size_t col);

    size_t nrow() const;
    size_t ncol() const;

    size_t size() const;
    double buffer(size_t i) const;
    std::vector<double> buffer_vector() const;

    //Return pointer to Data
    double * data();
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

/*
 * Naive matrix matrix multiplication.
 */
//Matrix operator*(Matrix const & mat1, Matrix const & mat2)
//{
    //if (mat1.ncol() != mat2.nrow())
    //{
        //throw std::out_of_range(
            //"the number of first matrix column "
            //"differs from that of second matrix row");
    //}

    //Matrix ret(mat1.nrow(), mat2.ncol());

    //for (size_t i=0; i<ret.nrow(); ++i)
    //{
        //for (size_t k=0; k<ret.ncol(); ++k)
        //{
            //double v = 0;
            //for (size_t j=0; j<mat1.ncol(); ++j)
            //{
                //v += mat1(i,j) * mat2(j,k);
            //}
            //ret(i,k) = v;
        //}
    //}

    //return ret;
//}

//std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
//{
    //for (size_t i=0; i<mat.nrow(); ++i)
    //{
        //ostr << std::endl << " ";
        //for (size_t j=0; j<mat.ncol(); ++j)
        //{
            //ostr << " " << std::setw(2) << mat(i, j);
        //}
    //}

    //return ostr;
//}

