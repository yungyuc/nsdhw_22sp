#!/usr/bin/env python3

from random  import randint, random
from timeit  import timeit

from _matrix  import Matrix, multiply_naive, multiply_tile, multiply_mkl

EPS = 1e-6

def _feq(a, b):
    return abs(a - b) < EPS

def _data_matrix(data):
    assert isinstance(data, list)

    assert (not data) or all(len(datacol) == len(data[0])  for datacol in data)
    
    N, M = len(data), len(data[0])
    mat = Matrix(N, M)
    for i in range(N):
        for j in range(M):
            mat[i, j] = data[i][j]
    
    return mat

def _matrix_data(mat):
    assert isinstance(mat, Matrix)

    N, M = mat.nrow, mat.ncol
    data = [[0  for j in range(M)]  for i in range(N)]
    for i in range(N):
        for j in range(M):
            data[i][j] = mat[i, j]
    
    return data

def _matrix_eq(lhs, rhs):
    assert isinstance(lhs, Matrix)
    assert isinstance(rhs, Matrix)

    if (lhs.nrow, lhs.ncol) != (rhs.nrow, rhs.ncol):
        return False
    
    for i in range(lhs.nrow):
        for j in range(lhs.ncol):
            if not _feq(lhs[i, j], rhs[i, j]):
                return False
    
    return True

def _multiply_naive_eq(listA, listB, listRes):
    return _matrix_eq(
            multiply_naive(_data_matrix(listA), _data_matrix(listB)),
            _data_matrix(listRes)
        )

def _multiply_tile_eq(listA, listB, listRes):
    return _matrix_eq(
            multiply_tile(_data_matrix(listA), _data_matrix(listB)),
            _data_matrix(listRes)
        )

def _multiply_mkl_eq(listA, listB, listRes):
    return _matrix_eq(
            multiply_mkl(_data_matrix(listA), _data_matrix(listB)),
            _data_matrix(listRes)
        )

def _multiply_all_eq(listA, listB, listRes):
    return all( mulfunc_eq(listA, listB, listRes)
            for mulfunc_eq in (_multiply_naive_eq, _multiply_tile_eq, _multiply_mkl_eq)
        )

def _gen_random_list(N, M, rng):
    data = [[0  for j in range(M)]  for i in range(N)]
    for i in range(N):
        for j in range(M):
            data[i][j] = rng()
    
    return data

def _gen_random_matrix(N, M, rng):
    mat = Matrix(N, M)
    for i in range(N):
        for j in range(M):
            mat[i, j] = rng()
    
    return mat

_small_test_1 = [
    [[1, 2], [3, 4]], [[9, 8], [7, 6]],
    [[23, 20], [55, 48]]
]

_small_test_2 = [
    [[12, 34], [78, 56]], [[13, 68], [24, 57]],
    [[972, 2754], [2358, 8496]]
]

_small_test_3 = [
    [[6, 3], [1, 7], [9, 2]], [[2, 3, 6], [8, 5, 3]],
    [[36, 33, 45], [58, 38, 27], [34, 37, 60]]
]

_small_test_4 = [
    [[2, 3, 6], [8, 5, 3]], [[6, 3], [1, 7], [9, 2]],
    [[69, 39], [80, 65]]
]

def test_multiply_naive_small_matrices():
    assert _multiply_naive_eq(*_small_test_1)
    assert _multiply_naive_eq(*_small_test_2)
    assert _multiply_naive_eq(*_small_test_3)
    assert _multiply_naive_eq(*_small_test_4)

def test_multiply_tile_small_matrices():
    assert _multiply_tile_eq(*_small_test_1)
    assert _multiply_tile_eq(*_small_test_2)
    assert _multiply_tile_eq(*_small_test_3)
    assert _multiply_tile_eq(*_small_test_4)

def test_multiply_mkl_small_matrices():
    assert _multiply_mkl_eq(*_small_test_1)
    assert _multiply_mkl_eq(*_small_test_2)
    assert _multiply_mkl_eq(*_small_test_3)
    assert _multiply_mkl_eq(*_small_test_4)

def test_random_int_matrices_equal_naive():
    # rng = lambda: randint(valRange[0], valRange[1]-1) + random()
    valRange = (-1000, 1000)
    rng = lambda: randint(valRange[0], valRange[1])

    for matSize in range(1, 1000, 100):
        listA = _gen_random_list(matSize, matSize, rng)
        listB = _gen_random_list(matSize, matSize, rng)

        matA = _data_matrix(listA)
        matB = _data_matrix(listB)

        matNaiveRes = multiply_naive(matA, matB)
        assert _multiply_all_eq(listA, listB, _matrix_data(matNaiveRes))

def benchmark():
    valRange = (-1000, 1000)
    rng = lambda: randint(valRange[0], valRange[1])
    sucess = True

    for matSize in range(1, 1000, 100):
        matA = _gen_random_matrix(matSize, matSize, rng)
        matB = _gen_random_matrix(matSize, matSize, rng)
        
        resMat = {}
        def setResult(name, matrix):  # wrapper since we need assignment in lambda
            resMat[name] = matrix

        naiveTime = timeit(lambda: setResult('naive', multiply_naive(matA, matB)), number=1, globals=locals())
        tileTime = timeit(lambda: setResult('tile', multiply_tile(matA, matB)), number=1, globals=locals())
        mklTime = timeit(lambda: setResult('mkl', multiply_mkl(matA, matB)), number=1, globals=locals())

        naivePass = 'model'  # model solution
        tilePass = 'pass'  if _matrix_eq(resMat['tile'], resMat['naive']) else 'fail'
        mklPass = 'pass'  if _matrix_eq(resMat['mkl'], resMat['naive']) else 'fail'

        print('Benchmark with matrix size = {}:'.format(matSize))
        print('{name:<5}: result = {res:>5}, time = {time:<9f} seconds'.format(name='naive', res=naivePass, time=naiveTime))
        print('{name:<5}: result = {res:>5}, time = {time:<9f} seconds'.format(name='tile', res=tilePass, time=tileTime))
        print('{name:<5}: result = {res:>5}, time = {time:<9f} seconds'.format(name='mkl', res=mklPass, time=mklTime))
        print(flush=True)

        sucess &= (tilePass == 'pass') and (mklPass == 'pass')
    
    if not sucess:
        raise AssertionError("at least one test case failed")
    

if __name__ == "__main__":
    benchmark()

