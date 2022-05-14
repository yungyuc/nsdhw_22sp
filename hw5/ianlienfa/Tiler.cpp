#include "Tiler.h"
Tiler::Tiler(int tile_size)
{
    m_tile_size = tile_size;
    m_mat1 = new Matrix(tile_size, tile_size);
    m_mat2 = new Matrix(tile_size, tile_size);
    m_res = new Matrix(tile_size, tile_size);
}

void Tiler::load(const Matrix & mat1, int row1, int col1, const Matrix &mat2, int row2, int col2)
{    
    // Load m_mat1 (row-based)
    int Base_col1 = col1 * m_tile_size;
    int Base_row1 = row1 * m_tile_size;        
    for(int i = 0; i < m_tile_size; i++){
        int Base_i = i * m_tile_size;
        for(int j = 0; j < m_tile_size; j++)
        {   
            m_mat1->buffer[Base_i + j] = mat1(Base_row1 + i, Base_col1 + j);
        }
    }
    int Base_row2 = row2 * m_tile_size;
    int Base_col2 = col2 * m_tile_size;
    for(int i = 0; i < m_tile_size; i++)
    {
        int Base_i = i * m_tile_size;
        for(int j = 0; j < m_tile_size; j++)
        {
            m_mat2->buffer[Base_i + j] = mat2(Base_row2 + j, Base_col2 + i);
        }
    }
}

Matrix Tiler::multiply()
{
    // Multiply m_mat1 and m_mat2
    for(int i = 0; i < m_tile_size; i++)
    {
        int Base_i = i * m_tile_size;
        for(int k = 0; k < m_tile_size; k++)
        {
            double accu = 0;
            int Base_k = k * m_tile_size;
            for(int j = 0; j < m_tile_size; j++)
            {
                accu += m_mat1->buffer[Base_i + j] * m_mat2->buffer[Base_k + j];
            }
            (*m_res)(i, k) = accu;       
            // cout << "(" << i << ", " << k << ") = " << accu << endl;
        }
    }
    return *m_res;
}

Tiler::~Tiler()
{
    delete m_mat1;
    delete m_mat2;
    delete m_res;
}