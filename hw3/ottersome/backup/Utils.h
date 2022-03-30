#include "matrix.h"

template<size_t T>
struct RowTile{

    //We can 
    RowTile(Matrix & result_matrix){
        //Load the first matrix in row major

    }

    inline RowTile & add(const Matrix & mat1, const Matrix & mat2, size_t tile_row, size_t  col_row){
        //Add both tiles, assume row major
        
        //We can have our already allocated space here
        //As we perform dot product on the tiles. We Simply add the result to the 
        //already allocated tile
        
        //Actual indices
        //rm_row = tile_row

        //Dot Product
        //For every tile 1 row
        for(size_t t1r= 0;m1r<T;m1r++){
            //For every tile 2 row
            for(size_t m2c;m2c<T;m2c++){
                double = mat1[]

            }
        }
    }
    void operator*(){
        //DO a dot product of tiles
    }

    Matrix & result;

    size_t nrows = 0;
    size_t ncols = 0;

} 
