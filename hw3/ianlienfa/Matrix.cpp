#include <iostream>
#include "Matrix.h"

int main()
{
    Matrix mtrx(3, 2, {
        1, 1,
        0, 0,
        0, 0       
    });  

    Matrix mtrx2(2, 2, {
        1, 0,
        1, 0,        
    });  
    mtrx2.show("mtrx2");    
    mtrx2.transpose();
    mtrx2.show("mtrx2_trans");    

    Matrix mtrx2_cpy = mtrx2;
    mtrx2_cpy.show("mtrx2_trans");    
    mtrx2.transpose();  
    mtrx2_cpy = mtrx2;  
    // for(int i = 0; i < mtrx2.mCols * mtrx2.mRows; i++)
    //     cout << mtrx2.buffer[i] << " ";
    //     cout << endl;

    mtrx2_cpy.show("mtrx3");
    // for(int i = 0; i < mtrx2.mCols * mtrx2.mRows; i++)
    //     cout << mtrx2_cpy.buffer[i] << " ";
    //     cout << endl;

    // Matrix res2 = mtrx * mtrx2;
    // res2.show();
}