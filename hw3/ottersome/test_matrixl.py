import numpy as np
import pytest as pt
import sys
import unittest
sys.path.append('./modules')
import _matrix as mtx
import time
import random

random.seed(time.time_ns())

def print_matrix(m):
    for row in range(m.nrow):
        print(" | ",end="")
        for col in range(m.ncol):
            print("{:03}".format(m[row,col]),end=",")
        print(" | ",end="\n")

def test_indexing():
    m1 = mtx.ident(3,3)
    print("m[0,0]={}".format(m1[0,0]))
    print("m[1,1,]={}".format(m1[1,1]))
    print("m[1,0]={}".format(m1[1,0]))
    print("m[2,2]={}".format(m1[2,2]))

def tost_match_tile_to_naive():

    size = 100
    mat1, mat2, *_ = make_matrices(size)

    print("Oh initial mail man: m1_cols:{}, m2_rows:{}".format(mat1.ncol,mat2.nrow))
    ret_naive = mtx.multiply_naive(mat1, mat2)
    print("Oh last mail man: m1_cols:{}, m2_rows:{}".format(mat1.ncol,mat2.nrow))
    ret_mkl = mtx.multiply_tile(mat1, mat2,16)

    self.assertEqual(size, ret_naive.nrow)
    self.assertEqual(size, ret_naive.ncol)
    self.assertEqual(size, ret_mkl.nrow)
    self.assertEqual(size, ret_mkl.ncol)

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
            self.assertEqual(ret_naive[i,j], ret_mkl[i,j])


def test_t_mult_by_ident():
    print("Pun pun Optimized ")
    m1 = mtx.ident(16,16)
    m2 = mtx.Matrix(16,2)
    print("Oh mail man: m1_cols:{}, m2_rows:{}".format(m1.ncol,m2.nrow))
    
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
    print("Oh mail right before tile man: m1_cols:{}, m2_rows:{}".format(m1.ncol,m2.nrow))
    # Remember to use multiple of siz eof double for parameter
    # Usually sizeof(double)=8
    m3 = mtx.multiply_tile(m1,m2,16)
    print_matrix(m3)

def test_mult_by_ident():
    print("Pun pun")
    m1 = mtx.ident(3,3)
    m2 = mtx.Matrix(3,1)
    m2[0,0]=1
    m2[0,1]=2
    m2[0,2]=3
    m3 = mtx.multiply_naive(m1,m2)
    print_matrix(m3)

def test_tile_n_native_random():
    m1,m2,_ = make_random_matrices(15)

    m3_naive = mtx.multiply_naive(m1,m2)
    m3_tiled = mtx.multiply_tile(m1,m2,32)

    print("Naive result")
    print_matrix(m3_naive)
    print("Tiled result")
    print_matrix(m3_tiled)

    print("Comparing the matrices")
    comp_res = m3_naive==m3_tiled
    print("Comparison Boolean:{}".format(comp_res))

    assert  comp_res


def test_mkl_n_native_random():
    m1,m2,_ = make_random_matrices(15)

    m3_naive = mtx.multiply_naive(m1,m2)
    m3_mkl= mtx.multiply_mkl(m1,m2)

    print("Naive result")
    print_matrix(m3_naive)
    print("MKL result")
    print_matrix(m3_mkl)

    print("Comparing the matrices")
    comp_res = m3_naive==m3_mkl
    print("Comparison Boolean:{}".format(comp_res))

    assert  comp_res



def tost_times_identity():
    print("Testing identity")
    m1,m2,m0 = make_matrices(16)
    mi = mtx.ident(16,16)
    mi[0,1] = 1

    mm1 = mtx.multiply_naive(mi,m1)
    print("Naive Result")
    print_matrix(mm1)
    mm2 = mtx.multiply_tile(mi,m1,32)
    print("Tile Result")
    print_matrix(mm2)

    assert mm1 == mm2
    # m2 = mtx(3,3)
    # print_matrix(m1)
    print("Identity passed")


def make_random_matrices(size):

    mat1 = mtx.Matrix(size,size)
    mat2 = mtx.Matrix(size,size)
    mat3 = mtx.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = random.randint(0, 9)
            mat2[it, jt] = random.randint(0, 9)
            mat3[it, jt] = 0

    return mat1, mat2, mat3

def make_matrices(size):

    mat1 = mtx.Matrix(size,size)
    mat2 = mtx.Matrix(size,size)
    mat3 = mtx.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            #  mat2[it, jt] = it * size + jt + 1
            mat2[jt, it] = it * size + jt + 1
            mat3[it, jt] = 0

    return mat1, mat2, mat3
