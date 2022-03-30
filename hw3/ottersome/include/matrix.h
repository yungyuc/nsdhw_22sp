#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 


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

    double & operator() (size_t row, size_t col);
    double   operator() (size_t row, size_t col) const;

    //double get(size_t row, size_t col) const;
    double & get(size_t row, size_t col) ;

    size_t nrow() const;
    size_t ncol() const;

    size_t size() const;
    double buffer(size_t i) const;
    std::vector<double> buffer_vector() const;

    //Arithmetic Operations
    Matrix n_mult(const Matrix & mat2);
    template<size_t ts>
    Matrix t_mult(const Matrix & mat2){
        if (this->ncol() != mat2.nrow())
        {
            throw std::out_of_range(
                    "the number of first matrix column "
                    "differs from that of second matrix row");
        }

        //Create matrix to return 
        Matrix retMat(this->nrow(), mat2.ncol());
        retMat.zero_out();


        //Get size of tiles:
        //TODO handle the edge cases
        //size_t num_rt1 = std::ceil((sizeof(double)*m_nrow)/ts);
        //size_t num_ct1 = std::ceil((sizeof(double)*m_ncol)/ts);
        //size_t num_rt2 = (sizeof(double)*mat2.nrow())/ts;
        //size_t num_ct2 = std::ceil((sizeof(double)*mat2.ncol())/ts);

        size_t curCellHeight =0;
        size_t curCellWidth = 0;
        size_t operation_num= 0;
        //Go around Cells in increments of their respective size
        for(size_t rtile = 0;rtile<retMat.nrow();rtile+=ts){
            for(size_t ctile = 0;ctile< retMat.ncol();ctile+=ts){

                //Actually do cell by cell dot product
                curCellHeight =  std::min(ts, retMat.nrow()- rtile);
                curCellWidth = std::min(ts, retMat.ncol()- ctile);
                operation_num = std::min(ts, (this)->ncol()-ctile);
                std::cout << "Info: \n"
                    <<" curCellHeight:"<<curCellHeight<<"\n"
                    <<" curCellWidth:"<<curCellWidth<<"\n"
                    <<" operation_num:"<<operation_num<<"\n";
                //For each element of the dot product
                //Size of 
                for(size_t tile_row = 0;tile_row<curCellHeight;tile_row++){
                    for(size_t tile_col = 0;tile_col<curCellWidth;tile_col++){
                        for(size_t elem = 0; elem< operation_num;elem++){
                            std::cout << "Info: \n"
                                <<" tile_row:"<<tile_row<<"\n"
                                <<" tile_col:"<<tile_col<<"\n"
                                <<" indexer:"<<elem<<"\n"
                                <<" a_el:"<<(*this)(rtile+tile_row,ctile+tile_col+elem)<<"\n"
                                <<" b_el:"<<mat2(rtile+tile_row+elem,ctile+tile_col)<<"\n";
                            retMat(rtile+tile_row,ctile+tile_col) += (*this)(rtile+tile_row,ctile+elem)
                                * mat2(rtile+elem,ctile+tile_col);
                            //retMat(rtile,ctile) = (*this)(rtile+tile_row,ctile+tile_col) + mat2(ctile+tile_col,rtile+tile_row);
                        }
                    }
                }

            }

        }


        return retMat;


    }

    //Return pointer to Data
    double * data();

    //Helpers
    void zero_out();
    
    //Info
    void print_vals();
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

