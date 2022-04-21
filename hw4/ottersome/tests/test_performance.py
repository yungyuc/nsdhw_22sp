import numpy as np
import pytest as pt
import sys
import unittest
sys.path.append('./modules')
import _matrix as mtx
import time
import timeit
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
    assert m1[0,0] == 1
    assert m1[1,1] == 1
    assert m1[0,1] == 0
    assert m1[1,0] == 0

def test_nomatch_tile_to_naive():

    size = 100
    mat1, mat2, *_ = make_matrices(size)

    ret_naive = mtx.multiply_naive(mat1, mat2)
    ret_mkl = mtx.multiply_tile(mat1, mat2,16)

    assert size == ret_naive.nrow
    assert size == ret_naive.ncol
    assert size == ret_mkl.nrow
    assert size == ret_mkl.ncol

    # Change one element and see if reports difference
    ret_naive[0,4] = 42.2

    matrices_differ =True

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            if ret_naive[i,j] != ret_mkl[i,j]:
                same_matrices=False
                break
        if same_matrices == False:
            break

    assert not same_matrices
    assert not (ret_naive == ret_mkl)

def test_match_tile_to_naive():

    size = 100
    mat1, mat2, *_ = make_matrices(size)

    ret_naive = mtx.multiply_naive(mat1, mat2)
    ret_mkl = mtx.multiply_tile(mat1, mat2,16)

    assert size == ret_naive.nrow
    assert size == ret_naive.ncol
    assert size == ret_mkl.nrow
    assert size == ret_mkl.ncol

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            assert ret_naive[i,j] == ret_mkl[i,j]
            #  assert mat1[i,j] != ret_mkl[i,j]
    assert ret_naive == ret_mkl

def test_t_mult_by_ident():
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
    
    m3 = mtx.multiply_tile(m1,m2,16)
    for i in range(2):
        for j in range (16):
            assert m3[i,j] == m2[i,j]

    assert  m3 == m2


def test_mult_by_ident():
    m1 = mtx.ident(3,3)
    m2 = mtx.Matrix(3,1)
    m2[0,0]=1
    m2[0,1]=2
    m2[0,2]=3
    m3 = mtx.multiply_naive(m1,m2)
    print_matrix(m3)
    assert m2 == m3

def test_tile_n_native_random():
    m1,m2,_ = make_random_matrices(random.randint(3,31))

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
    m1,m2,_ = make_random_matrices(random.randint(3,31))

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

def test_benchmark():

    # Since this is already provided by the `validate.py` script I'll just put it here
    setup = '''
import _matrix as mtx
import random
import time

size = 1000
random.seed(time.time_ns())

mat1 = mtx.Matrix(size,size)
mat2 = mtx.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = random.gauss(0, 1)
        mat2[it, jt] = random.gauss(0, 1)
'''

    naive = timeit.Timer('mtx.multiply_naive(mat1, mat2)', setup=setup)
    mkl = timeit.Timer('mtx.multiply_mkl(mat1, mat2)', setup=setup)
    tile = timeit.Timer('mtx.multiply_tile(mat1, mat2,16)', setup=setup)

    repeat = 5

    naivesec = min(naive.repeat(repeat=repeat, number=1))
    tile_sec = minsec = min(tile.repeat(repeat=repeat, number=1))
    mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
    with open('performance.txt', 'w') as fobj:
        for w in [sys.stdout,fobj]:
            w.write(f'Start multiply_naive (repeat={repeat}), take min = ')
            w.write(f'{naivesec} seconds\n')

            w.write(f'Start multiply_tile (repeat={repeat}), take min = ')
            w.write(f'{tile_sec} seconds\n')

            w.write(f'Start multiply_mkl (repeat={repeat}), take min = ')
            w.write(f'{mklsec} seconds\n')

            w.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))
            w.write('Tile speed-up over naive: %g x\n' % (naivesec/tile_sec))
            w.write('MKL speed-up over Tile: %g x\n' % (tile_sec/mklsec))


    pass 

