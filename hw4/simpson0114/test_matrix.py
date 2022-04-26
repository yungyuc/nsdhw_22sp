import _matrix
import pytest
import timeit
import numpy as np
import random

def matrix_init():
    x = random.randint(1, 300)
    y = random.randint(1, 300)
    z = random.randint(1, 300)
    tsize = random.randint(1, 16)

    _m1 = _matrix.Matrix(x, y)
    _m2 = _matrix.Matrix(y, z)
    _matrix.initialize(_m1)
    _matrix.initialize(_m2)

    return x, y, z, tsize, _m1, _m2

def matrix_init_big():
    x, y, z, tsize = 1000, 1000, 1000, 16

    _m1 = _matrix.Matrix(x, y)
    _m2 = _matrix.Matrix(y, z)
    _matrix.initialize(_m1)
    _matrix.initialize(_m2)

    return x, y, z, tsize, _m1, _m2


def test_matrix_implement():
    x, y, z, tsize, _m1, _m2 = matrix_init()

    naive = _matrix.multiply_naive(_m1, _m2)
    tile = _matrix.multiply_tile(_m1, _m2, tsize)
    mkl = _matrix.multiply_mkl(_m1, _m2)

    for i in range(x):
        for j in range(z):
            assert pytest.approx(naive[i, j]) == pytest.approx(mkl[i, j])
            assert pytest.approx(tile[i, j]) == pytest.approx(mkl[i, j])


def test_performance():
    x, y, z, tsize, _m1, _m2 = matrix_init_big()

    init = dict(_matrix=_matrix, _m1=_m1, _m2=_m2, tsize=tsize)
    naive = timeit.timeit(
        "_matrix.multiply_naive(_m1, _m2)", number=3, globals=init)
    tile = timeit.timeit(
        "_matrix.multiply_tile(_m1, _m2, tsize)", number=3, globals=init)
    mkl = timeit.timeit("_matrix.multiply_mkl(_m1, _m2)", number=3, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"multiply_naive: {naive:.4f} seconds.\n")
        f.write(f"multiply_tile: {tile:.4f} seconds.\n")
        f.write(f"multiply_mkl: {mkl:.4f} seconds.\n")