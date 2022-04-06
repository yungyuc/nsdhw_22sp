import _matrix
import pytest
import timeit
import random

def test_costructor():
    ROWSIZE=7
    COLSIZE=7

    M = _matrix.Matrix(ROWSIZE, COLSIZE)

    TestResult = True

    for i in range(ROWSIZE):
        for j in range(COLSIZE):
            if M[i,j] != 0.0:
                TestResult = False
                break

    assert TestResult

def test_size():
    ROWSIZE=5
    COLSIZE=4

    A = _matrix.Matrix(ROWSIZE, COLSIZE)
    B = _matrix.Matrix(COLSIZE, ROWSIZE)

    C = _matrix.multiply_naive(A, B)
    assert C.nrow == ROWSIZE
    assert C.ncol == ROWSIZE

def test_multiplication():
    ROWSIZE=1000
    COLSIZE=1000
    BOKSIZE=16

    A = _matrix.Matrix(ROWSIZE, COLSIZE)
    B = _matrix.Matrix(ROWSIZE, COLSIZE)

    for i in range(ROWSIZE):
        for j in range(COLSIZE):
            A[i,j] = random.random()*10
            B[i,j] = random.random()*10

    Naive = _matrix.multiply_naive(A, B)
    Tile = _matrix.multiply_tile(A, B, BOKSIZE)
    Mkl = _matrix.multiply_mkl(A, B)

    assert Tile == Naive
    assert Mkl == Naive
