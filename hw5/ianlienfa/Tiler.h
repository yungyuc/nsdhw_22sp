#ifndef TILER_H
#define TILER_H
#include "matrix.hpp"

struct Matrix;
struct Tiler {
    int m_tile_size;
    Matrix *m_mat1;
    Matrix *m_mat2;
    Matrix *m_res;
    void load(const Matrix & mat1, int row1, int col1, const Matrix &mat2, int row2, int col2);
    Tiler(int tile_size);
    ~Tiler();
    Matrix multiply();
};


#endif