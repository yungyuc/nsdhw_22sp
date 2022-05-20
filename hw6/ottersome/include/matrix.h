#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 
#include <chrono>
#include <pybind11/numpy.h>
using namespace std::chrono;

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

    double const & operator() (size_t row, size_t col) const{
        return m_buffer[row*m_ncol + col];
    };
    double & operator() (size_t row, size_t col){
        return m_buffer[row*m_ncol + col];
    }

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
    
    //template<size_t ts_bytes>
    Matrix multiply_tile(const Matrix & mat2, size_t ts) const{

        // auto start = high_resolution_clock::now();
        if (this->ncol != mat2.nrow)
        {
            throw std::out_of_range(
                    "the number of first matrix column "
                    "differs from that of second matrix row");
        }

        //Create matrix to return and zero out its elements
        Matrix retMat(this->nrow, mat2.ncol);
        retMat.zero_out();

        size_t tile_vbound =0, tile_hbound=0,op_bound=0;
        // auto b4_loop = high_resolution_clock::now();

        // auto aft_loop = high_resolution_clock::now();
        
        //Go around Cells in increments of their respective size
        for(size_t rtile = 0; rtile < this->nrow;rtile+=ts)
        {
            tile_vbound =  std::min(ts+rtile, retMat.nrow);
            for(size_t t_col = 0;t_col< mat2.ncol;t_col+=ts)
            {
                tile_hbound = std::min(ts+t_col, mat2.ncol);
                //For every vertical tile in matrix 2(of the ctil tile column)
                for(size_t vtile=0;vtile<mat2.nrow;vtile+=ts)
                {
                    //Actually do cell by cell dot product
                    op_bound = std::min(ts+vtile, (this)->ncol);
                    for(size_t elem = vtile; elem< op_bound;elem++)
                    {
                        for(size_t row = rtile;row<tile_vbound;row++)
                        {
                            for(size_t col = t_col;col<tile_hbound;col++)
                            {
                                // rtr = rtile+row;
                                retMat(row,col)  += (*this)(row,elem)
                                    * mat2(elem,col);
                            }
                        }
                    }
                }

            }

        }

        // auto prep_duration = duration_cast<milliseconds>(b4_loop-start);
        // auto loop_duration = duration_cast<milliseconds>(aft_loop-b4_loop);
        // auto tot_duration = duration_cast<milliseconds>(aft_loop-start);
        // std::cout << "Execution Times:\n"
        //     <<"\tTotalDuration"<<tot_duration.count()<<"\n"
        //     <<"\tprep_duration"<<prep_duration.count()<<"\n"
        //     <<"\tloop_duration"<<loop_duration.count()<<"\n"
        //     <<std::endl;

        return retMat;

    }

    //
    pybind11::array_t<double> get_ndarray();

    //Return pointer to Data
    double * data();
    double * data() const;

    //Helpers
    void zero_out();
    
    //Info
    void print_vals();

};

