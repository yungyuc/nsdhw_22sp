#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 


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

    //double get(size_t row, size_t col) const;
    double & get(size_t row, size_t col) ;

    //For convenience
    const size_t  & nrow;
    const size_t  & ncol;

    //For python members
    size_t get_nrow() { 
        std::cout << "For some reason this breaks"<<std::endl;
        return m_nrow;}
    size_t get_ncol() { return m_ncol;}

    size_t size() const;
    double buffer(size_t i) const; std::vector<double> buffer_vector() const;

    //Arithmetic Operations
    Matrix multiply_naive(const Matrix & mat2);
    Matrix multiply_mkl(const Matrix & mat2);
    //static Matrix n_mult(const Matrix & mat1, const Matrix & mat2){
        //return mat1.n_mult(mat2);
    //}
    //template<size_t ts_bytes>
    Matrix multiply_tile(const Matrix & mat2, size_t ts_bytes){
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

        //Get size of tiles:
        //TODO handle the edge cases
        //size_t num_rt1 = std::ceil((sizeof(double)*m_nrow)/ts);
        //size_t num_ct1 = std::ceil((sizeof(double)*m_ncol)/ts);
        //size_t num_rt2 = (sizeof(double)*mat2.nrow())/ts;
        size_t num_ct2 = std::ceil((float)(sizeof(double)*mat2.ncol)/ts_bytes);
        //std::cout << "Dameloo : "<<num_ct2<<"<<as opposed to : "<<(double)(sizeof(double)*mat2.ncol())/ts_bytes<<std::endl;

        size_t rows =0;
        size_t cols = 0;
        size_t ops= 0;

        std::cout <<"Meepo"<<std::endl;
        //Go around Cells in increments of their respective size
        for(size_t rtile = 0;rtile<retMat.nrow;rtile+=ts){

            for(size_t t_col = 0;t_col< num_ct2;t_col+=ts){
                std::cout <<"sarancho"<<std::endl;
                //For every vertical tile in matrix 2(of the ctil tile column)
                for(size_t vtile=0;vtile<mat2.nrow;vtile+=ts){
                    //Actually do cell by cell dot product
                    rows =  std::min(ts, retMat.nrow- rtile);
                    cols = std::min(ts, mat2.ncol- t_col);
                    ops = std::min(ts, (this)->ncol-vtile);
                    std::cout << "Info: \n"
                        <<" rtile:"<<rtile<<"\n"
                        <<" t_col:"<<t_col<<"\n"
                        <<" vtile:"<<vtile<<"\n"
                        <<" cols:"<<cols<<"\n"
                        <<" rows:"<<rows<<"\n"
                        <<" ops_num:"<<ops<<"\n";
                    //For each element of the dot product
                    //Size of 
                    for(size_t col = 0;col<cols;col++){
                        for(size_t row = 0;row<rows;row++){
                            std::cout << "Info: \n"
                                <<" tile_row:"<<row<<"\n"
                                <<" tile_col:"<<col<<"\n"<<std::endl;
                            for(size_t elem = 0; elem< ops;elem++){
                                std::cout
                                    <<"\tindexer:"<<elem<<"\n"
                                    <<"\tprod="<<(*this)(rtile+row,vtile+elem)
                                    <<"*"<<mat2(vtile+elem,t_col+col)<<std::endl;
                                    //<<"\ta_el:"<<(*this)(rtile+row,vtile+elem)<<"\n"
                                    //<<"\tb_el:"<<mat2(vtile+elem,t_col+col)<<"\n";
                                retMat(rtile+row,t_col+col)  += (*this)(rtile+row,vtile+elem)
                                    * mat2(vtile+elem,t_col+col);
                                //retMat(rtile+tile_row,ctile+tile_col) += (*this)(rtile+tile_row,ctile+elem)
                                    //* mat2(rtile+elem,ctile+tile_col);
                                //retMat(rtile,ctile) = (*this)(rtile+tile_row,ctile+tile_col) + mat2(ctile+tile_col,rtile+tile_row);
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

