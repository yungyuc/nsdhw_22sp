import pytest
import _matrix
import random

def test_IfResultsAreTheSame():
    dim = 1200
    m1 = _matrix.Matrix(dim,dim)
    m2 = _matrix.Matrix(dim,dim)
    for i in range(m1.nrow):
        for j in range(m1.ncol):
            m1[i,j] = random.random()*5
            m2[i,j] = random.random()*5

    m3 = _matrix.multiply_tile(m1,m2,64)
    m4 = _matrix.multiply_naive(m1,m2)
    assert m3 == m4

def test_MatrixContructor():
    dim = 10
    m1 = _matrix.Matrix(dim,dim)
    AllElementsAreZero = True
    for r in range(dim):
        for c in range(dim):
            if  m1[r, c] != 0.0:
                AllElementsAreZero = False
                break
        if not AllElementsAreZero:
            break
    assert AllElementsAreZero

    dim = 2
    m2 = _matrix.Matrix(dim,dim,[1.0, 2.0, 3.0, 4.0])
    assert m2[0, 0] == 1.0
    assert m2[0, 1] == 2.0
    assert m2[1, 0] == 3.0
    assert m2[1, 1] == 4.0

def test_MatrixOperation():
    r = 10
    k = 9
    c = 8
    m1 = _matrix.Matrix(r, k)
    m2 = _matrix.Matrix(k, c)

    m1_mul_2 = _matrix.multiply_naive(m1, m2)
    assert m1_mul_2.nrow == r
    assert m1_mul_2.ncol == c

    with pytest.raises(Exception):
        m2_mul_1 = _matrix.multiply_naive(m2, m1)
    
    m3 = _matrix.Matrix(r, k, [random.random()*5 for it in range(r*k)])
    m4 = _matrix.Matrix(r, k)
    
    assert m1 != m3
    assert m1 == m4
