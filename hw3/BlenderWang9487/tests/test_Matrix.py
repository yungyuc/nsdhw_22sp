import pytest
import _matrix
import random

def test_tile():
    m1 = _matrix.Matrix(1000,1000)
    dif = 0.0
    for i in range(m1.nrow):
        for j in range(m1.ncol):
            m1[i,j] = random.randint(-5,5)
    m2 = _matrix.Matrix(1000,1000)
    dif = 0.0
    for i in range(m1.nrow):
        for j in range(m1.ncol):
            m2[i,j] = random.randint(-5,5)
    
    dif = 0.0
    m3 = _matrix.multiply_tile(m1,m2,1000)
    m4 = _matrix.multiply_mkl(m1,m2)
    sum = 0.0
    sum_c = 0
    for i in range(m3.nrow):
        for j in range(m3.ncol):
            if m3[i,j] - m4[i,j] != 0.0:
                sum += abs(m3[i,j] - m4[i,j])
                sum_c += 1
    print("Aver err:",sum/sum_c,", count",sum_c)
    # assert flag

test_tile()