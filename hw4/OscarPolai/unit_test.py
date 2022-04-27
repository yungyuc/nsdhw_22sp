import pytest
import _matrix
import numpy as np
import random
import unittest

# create matrix within random number  

def matrix_ini(row, col):

    _m = _matrix.Matrix(row, col)

    m = np.random.rand(row, col)

    for i in range(row):
        for j in range(col):
            _m[i,j] = m[i,j]
            print(_m[i, j])
                    
    return m, _m



# test simple naive mulitplication verification

def test_naive_verify():
    m1, _m1 = matrix_ini(3, 2)
    m2, _m2 = matrix_ini(2, 4)
    
    _naive = _matrix.multiply_naive(_m1, _m2)
    npdot = np.dot(m1, m2)

    for i in range(3):
        for j in range(4):
            assert _naive[i, j] == npdot[i, j]
    

def test_memory():
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    m, _m1 = matrix_ini(random.randint(1,10), random.randint(1,10))
    r = _m1.nrow
    c = _m1.ncol

    assert _matrix.bytes() == (r * c * 8)
    assert _matrix.allocated() == (r * c * 8) + base_alloc  
    assert _matrix.deallocated() ==  base_dealloc

