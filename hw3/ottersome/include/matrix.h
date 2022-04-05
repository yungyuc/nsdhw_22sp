#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 

#define EQ_DIFF 1.0e-5

class Matrix {
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

public:


    Matrix(size_t nrow, size_t ncol);
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);

    Matrix & operator=(std::vector<double> const & vec);
    Matrix(Matrix const & other);
    Matrix & operator=(Matrix const & other);
    Matrix(Matrix && other);

    Matrix & operator=(Matrix && other);
    ~Matrix();

    double & operator() (size_t row, size_t col);
    double   operator() (size_t row, size_t col) const;

    bool operator==(const Matrix & other) const;

    //double get(size_t row, size_t col) const;
    double & get(size_t row, size_t col) ;

    //For convenience
    const size_t  & nrow;
    const size_t  & ncol;

    //For python members
    size_t get_nrow() const{ return m_nrow;}
    size_t get_ncol() const { return m_ncol;}

    size_t size() const;
    double buffer(size_t i) const; std::vector<double> buffer_vector() const;

    //Arithmetic Operations
    Matrix multiply_naive(const Matrix & mat2) const;
    Matrix multiply_mkl(const Matrix & mat2) const;
    //static Matrix n_mult(const Matrix & mat1, const Matrix & mat2){
        //return mat1.n_mult(mat2);
    //}
    //template<size_t ts_bytes>
    Matrix multiply_tile(const Matrix & mat2, size_t ts_bytes) const{

        // std::cout << "Number m1 cols"<< this->ncol
        //     <<"\nNumebr of m2 rows:"<<mat2.nrow<<std::endl;
        if (this->ncol != mat2.nrow)
        {
            throw std::out_of_range(
                    "the number of first matrix column "
                    "differs from that of second matrix row");
        }

        //Create matrix to return 
        Matrix retMat(this->nrow, mat2.ncol);
        retMat.zero_out();

        size_t ts = ts_bytes/sizeof(double);

        // size_t num_ct2 = std::ceil((double)(sizeof(double)*mat2.ncol)/ts_bytes);

        size_t rows =0;
        size_t cols = 0;
        size_t ops= 0;

        size_t rtr = 0;;
        size_t tcc = 0;;

        //Go around Cells in increments of their respective size
        for(size_t rtile = 0;rtile<retMat.nrow;rtile+=ts){

            rows =  std::min(ts, retMat.nrow- rtile);
            for(size_t t_col = 0;t_col< mat2.ncol;t_col+=ts){
                cols = std::min(ts, mat2.ncol- t_col);
                //For every vertical tile in matrix 2(of the ctil tile column)
                for(size_t vtile=0;vtile<mat2.nrow;vtile+=ts){
                    //Actually do cell by cell dot product
                    ops = std::min(ts, (this)->ncol-vtile);
                    for(size_t col = 0;col<cols;col++){
                        tcc = t_col+col;
                        for(size_t row = 0;row<rows;row++){
                            rtr = rtile+row;
                            for(size_t elem = 0; elem< ops;elem++){
                                retMat(rtr,tcc)  += (*this)(rtr,vtile+elem)
                                    * mat2(vtile+elem,tcc);
                            }
                        }
                    }
                }

            }

        }


        return retMat;


    }

    //Return pointer to Data
    double * data();
    double * data() const;

    //Helpers
    void zero_out();
    
    //Info
    void print_vals();

};

