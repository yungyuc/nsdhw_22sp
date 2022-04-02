import numpy as np
import pytest as pt
import sys
import unittest
sys.path.append('./modules')
import _matrix as mtx

def print_matrix(m):
    for row in range(m.nrow()):
        print(" | ",end="")
        for col in range(m.ncol()):
            print(m[row,col],end=",")
        print(" | ",end="\n")

def test_indexing():
    m1 = mtx.ident(3,3)
    print("m[0,0]={}".format(m1[0,0]))
    print("m[1,1,]={}".format(m1[1,1]))
    print("m[1,0]={}".format(m1[1,0]))
    print("m[2,2]={}".format(m1[2,2]))

def test_match_tile_to_naive():

    size = 100
    mat1, mat2, *_ = make_matrices(size)

    ret_naive = _matrix.multiply_naive(mat1, mat2)
    ret_mkl = _matrix.multiply_tile(mat1, mat2)

    self.assertEqual(size, ret_naive.nrow)
    self.assertEqual(size, ret_naive.ncol)
    self.assertEqual(size, ret_mkl.nrow)
    self.assertEqual(size, ret_mkl.ncol)

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
            self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

def test_matrix_random_coeff():

def test_t_mult_by_ident():
    print("Pun pun Optimized ")
    m1 = mtx.ident(16,16)
    m2 = mtx.Matrix(16,2)
    
    # m1[0,3] = 1
    m2[0,1]=1
    m2[1,1]=2
    m2[2,1]=3
    m2[3,1]=4
    m2[4,1]=5
    m2[5,1]=6
    m2[6,1]=7
    m2[7,1]=8
    m2[8,1]=9
    m2[9,1]=10
    m2[10,1]=11
    m2[11,1]=12
    m2[12,1]=13
    m2[13,1]=14
    m2[14,1]=15
    m2[15,1]=16

    m2[0,0]=1
    m2[1,0]=2
    m2[2,0]=3
    m2[3,0]=4
    m2[4,0]=5
    m2[5,0]=6
    m2[6,0]=7
    m2[7,0]=8
    m2[8,0]=9
    m2[9,0]=10
    m2[10,0]=11
    m2[11,0]=12
    m2[12,0]=13
    m2[13,0]=14
    m2[14,0]=15
    m2[15,0]=16
    m3 = m1.t_mult(m2)
    print_matrix(m3)
def test_mult_by_ident():
    print("Pun pun")
    m1 = mtx.ident(3,3)
    m2 = mtx.Matrix(3,1)
    m2[0,0]=1
    m2[0,1]=2
    m2[0,2]=3
    m3 = m1.n_mult(m2)
    print_matrix(m3)

def test_times_identity():
    print("Testing identity")
    m1 = mtx.ident(3,3)
    # m2 = mtx(3,3)
    # print_matrix(m1)
    print("M1 has been initialized")
    pass


def make_matrices(self, size):

    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    mat3 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0

    return mat1, mat2, mat3
